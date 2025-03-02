#pragma once

#include "TurretPawn.h"
#include "InputAction.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UCapsuleComponent;
class USphereComponent;
class UHealthComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoDepletedDelegate, int32, AmmoCount);

UCLASS()
class TOWEROFFENCE_API ATankPawn : public ATurretPawn
{
    GENERATED_BODY()

public:
    ATankPawn(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void ReloadAmmo();

    int32 GetCurrentAmmo() const { return CurrentAmmo; }
    int32 GetMaxAmmo() const { return MaxAmmo; }

    UPROPERTY(BlueprintAssignable)
    FOnAmmoDepletedDelegate OnAmmoDepleted;

    FVector PreviousLocation;//
    FVector RealVelocity;//

protected:
    virtual void BeginPlay() override;

    virtual void PossessedBy(AController* NewController) override;

private:
    void MoveTank(const FInputActionInstance& Value);
    void TurnTank(const FInputActionInstance& Value);
    void TurnCamera(const FInputActionInstance& Value);
    void FireTower(const FInputActionInstance& Value);

    void HandleMoveEffects();
    void HandleTurnEffects();

    void FakeReload();

    float RotInput = 0.0f;
    float TurnInput = 0.0f;
    float CurrentSpeed = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float StoppingInertia = 100.0f; 

    //Amount of ammunition
    UPROPERTY(EditDefaultsOnly, Category = "Ammo")
    int32 MaxAmmo = 10; 

    UPROPERTY(EditDefaultsOnly, Category = "Ammo")
    int32 CurrentAmmo = 0; 

    FVector ForwardVector;
    FVector Acceleration;
    FVector CurrentVelocity;
    FVector SpawnLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent* Capsule = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera = nullptr;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent* HealthWidgetComp = nullptr;

    // MoveTank
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float AccelerationTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float AccelerationSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Speed = 300.0f;

    // TurnTank
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float TurnTracksSpeed = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float TurnTowerSpeed = 2.0f;

    // FireTower
    UPROPERTY(EditAnywhere, Category = "Shot")
    float ProjectileShotInterval = 0.2f;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_MoveTankAxis = nullptr;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_TurnTankAxis = nullptr;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_TurnCameraAxis = nullptr;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_FireAxis = nullptr;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* IMC_InputContext = nullptr;
};
