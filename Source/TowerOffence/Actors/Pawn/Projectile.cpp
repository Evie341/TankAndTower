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

        ApplyExplosionDamage();

        GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AProjectile::StopDeathEffects, TimeDeathEffects, false);

        bHasDealtDamage = true;
    }
}

void AProjectile::ApplyExplosionDamage()
{
    UGameplayStatics::ApplyRadialDamageWithFalloff(
        this,
        Damage,
        MinimalDamage,
        GetActorLocation(),
        ExplosionRadius->GetScaledSphereRadius() * 0.5f,// The radius in which the damage is maximum
        ExplosionRadius->GetScaledSphereRadius(),// Radius in which the damage is reduced to a minimum
        DamageParameter,
        DamageType,
        TArray<AActor*>(),
        this,
        GetInstigatorController()
    );
}

void AProjectile::StopDeathEffects()
{
    Destroy();
}
