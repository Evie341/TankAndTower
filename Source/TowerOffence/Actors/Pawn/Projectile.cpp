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

    // Aydio effects components
    ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MoveSound"));
    ExplosionSound->SetupAttachment(MeshComponent);
    ExplosionSound->bAutoActivate = false;

    // Visual effects components
    ExplosionEffectVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CurrentMoveVFX"));
    ExplosionEffectVFX->SetupAttachment(MeshComponent);
    ExplosionEffectVFX->bAutoActivate = false;
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

        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageType);

        GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AProjectile::StopDeathEffects, TimeDeathEffects, false);
        
        bHasDealtDamage = true;
    }
}

void AProjectile::StopDeathEffects()
{
    Destroy();
}
