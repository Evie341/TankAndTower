#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TOWEROFFENCE_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

    virtual void PostInitializeComponents() override;

    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UDamageType> DamageType;

private:
    bool bHasDealtDamage = false;

    void StopDeathEffects();
    void ApplyExplosionDamage();

    UPROPERTY(EditAnywhere, Category = "Audio")
    float TimeDeathEffects = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float Damage = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float MinimalDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float DamageParameter = 1.0f;

    FTimerHandle UnusedHandle;

    UPROPERTY(VisibleAnywhere, Category = "Category")
    USphereComponent* Root = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Category")
    UStaticMeshComponent* MeshComponent = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "ProjectileMovementComponent")
    UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* ExplosionSound = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* ExplosionEffectVFX = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Category")
    USphereComponent* ExplosionRadius = nullptr;
};
