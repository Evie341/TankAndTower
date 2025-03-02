#pragma once

#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerOffence/Actors/Player/MyPlayerState.h"
#include "DestructibleComponent.h" 
#include "GeometryCollection/GeometryCollectionComponent.h"

#include "TurretPawn.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UBoxComponent;
class USphereComponent;
class USceneComponent;
class UMaterialInstanceDynamic;
class FName;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnKilled, APawn*, PawnKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireDelegate);

UCLASS(ShowCategories = (Mobility), Meta = (BlueprintSpawnableComponent))
class TOWEROFFENCE_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	ATurretPawn(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

	void EndGame(bool bIsWinner);

	bool IsPawnAlive() const { return bIsAlive; }

	UGeometryCollectionComponent* GetBaseMesh() const { return BaseMesh; }
	UGeometryCollectionComponent* GetTurretMesh() const { return TurretMesh; }
	USceneComponent* GetProjectileSpawnPoint() const { return ProjectileSpawnPoint; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	float GetLastFireTime() const { return LastFireTime; }
	float GetShotInterval() const { return ShotInterval; }

	UPROPERTY(BlueprintAssignable)
	FOnPawnKilled OnPawnKilled;
		
	UPROPERTY(BlueprintAssignable)
	FOnFireDelegate OnFire;

	FTimerHandle UnusedHandle;

	FVector LastHitDirection;

protected:
	virtual void RotateTurret(const FVector& TargetDirection);

	void Tick(float DeltaTime);

	void Fire();
 
	virtual bool CanFire() const;	

	bool bIsTurretRotating = false; 
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, Category = "Audio")
	float FadeOutTime = 1.0f;

	// Audio/effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* StaticMoveSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* MoveSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* TurnSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* TowerTurnSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FireSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* DeathSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* StaticMoveVFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CurrentMoveVFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CurrentTurnVFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* CurrentTowerTurnVFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* FireVFXComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* DeathVFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> FireShake;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> DeathShake;

private:
	UFUNCTION()
	void HandleDeath();

	void StopDeathEffects();
	void RotateTurretoggleEffects(bool bShouldActivate);
	
	// Function for handling collision events
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	float LastFireTime = 0.0f;

	// FireTower
	UPROPERTY(EditAnywhere, Category = "ShotInterval")
	float ShotInterval = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	float TimeDeathEffects = 1.0f;

	// Destruction Collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction", meta = (AllowPrivateAccess = "Tower"))
	float ImpulseStrength = 1000.0f;

	// Variable to set the repulsion force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float RepelForce;

	UPROPERTY(EditAnywhere, Category = "SpawnProjectileBlueprint")
	TSubclassOf<AActor> SpawnProjectileBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* BaseComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxTower = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxBase = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chaos", meta = (AllowPrivateAccess = "true"))
	UGeometryCollectionComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chaos", meta = (AllowPrivateAccess = "true"))
	UGeometryCollectionComponent* TurretMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true", GetOptions = "GetBaseMeshMaterialSlotOptions"))
	FName BaseMeshMaterialSlot;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true", GetOptions = "GetTurretMeshMaterialSlotOptions"))
	FName TurretMeshMaterialSlot;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true", GetOptions = "GetBaseMeshColorParameterOptions"))
	FName BaseColorParameter;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true", GetOptions = "GetTurretMeshColorParameterOptions"))
	FName TurretColorParameter;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	FColor TeamColor = FColor::White;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterialBase = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterialTurret = nullptr;

	UMaterialInstanceDynamic* CreateAndSetDynamicMaterial(UMeshComponent* Mesh, FName Slot, FName ColorParameter);

	TArray<FString> GetMaterialSlotOptions(UGeometryCollectionComponent* GeometryComponent) const;
	TArray<FString> GetColorParameterOptions(UMeshComponent* MeshComponent, FName MaterialSlot) const;

	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlotOptions() const;

	UFUNCTION()
	TArray<FString> GetTurretMeshMaterialSlotOptions() const;

	UFUNCTION()
	TArray<FString> GetBaseMeshColorParameterOptions() const;

	UFUNCTION()
	TArray<FString> GetTurretMeshColorParameterOptions() const;
};
