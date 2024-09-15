#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "TowerOffence/Actors/Pawn/TankPawn.h"
#include "TowerOffence/Actors/Pawn/TowerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

AProjectile::AProjectile()
{
    Root = CreateDefaultSubobject<USphereComponent>(TEXT("BaseComponent"));
    SetRootComponent(Root);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(Root);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

    // Audio effects components
    ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MoveSound"));
    ExplosionSound->SetupAttachment(MeshComponent);
    ExplosionSound->bAutoActivate = false;

    // Visual effects components
    ExplosionEffectVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CurrentMoveVFX"));
    ExplosionEffectVFX->SetupAttachment(MeshComponent);
    ExplosionEffectVFX->bAutoActivate = false;

    // Add a sphere component for the explosion radius
    ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRadius"));
    ExplosionRadius->SetupAttachment(Root);
}

void AProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    Root->OnComponentHit.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bHasDealtDamage)
    {
        return;
    }
    if (OtherActor && (OtherActor != GetOwner()) && (OtherActor != GetInstigator()))
    {
        if (ExplosionSound)
        {
            ExplosionSound->Play();
        }
        if (ExplosionEffectVFX)
        {
            ExplosionEffectVFX->Activate(true);
        }
        if (MeshComponent)
        {
            MeshComponent->SetVisibility(false);
        }
        if (Root)
        {
            Root->SetSimulatePhysics(false);
            Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // Apply radial damage with falloff
        float MaxDistance = ExplosionRadius->GetScaledSphereRadius();
        float Distance = FVector::Dist(GetActorLocation(), OtherActor->GetActorLocation());
        float CalculatedDamage = CalculateDamage(Distance);

        UGameplayStatics::ApplyRadialDamage(
            this,
            CalculatedDamage,
            GetActorLocation(),
            MaxDistance,
            DamageType,
            TArray<AActor*>(),
            this,
            GetInstigatorController(),
            true, // bDoFullDamage
            ECC_Visibility
        );

        GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AProjectile::StopDeathEffects, TimeDeathEffects, false);

        bHasDealtDamage = true;
    }
}

// Damage reduction due to distance
float AProjectile::CalculateDamage(float Distance)
{
    float MaxDistance = ExplosionRadius->GetScaledSphereRadius();
    float DamageMultiplier = FMath::Clamp(1.0f - (Distance / MaxDistance), 0.0f, 1.0f);
    return Damage * DamageMultiplier;
}

void AProjectile::StopDeathEffects()
{
    Destroy();
}
