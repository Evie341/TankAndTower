#include "TankPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "TowerOffence/Components/HealthComponent.h"
#include "TowerOffence/UserInterface/HealthBarWidgetBase.h"
 
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "Components/BoxComponent.h"

ATankPawn::ATankPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass(TEXT("BaseComponent"), UBoxComponent::StaticClass()))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (IsValid(Input))
    {
        if (IsValid(IA_MoveTankAxis))
        {
            Input->BindAction(IA_MoveTankAxis, ETriggerEvent::Triggered, this, &ATankPawn::MoveTank);
        }
        if (IsValid(IA_TurnTankAxis))
        {
            Input->BindAction(IA_TurnTankAxis, ETriggerEvent::Triggered, this, &ATankPawn::TurnTank);
        }
        if (IsValid(IA_TurnCameraAxis))
        {
            Input->BindAction(IA_TurnCameraAxis, ETriggerEvent::Triggered, this, &ATankPawn::TurnCamera);
        }
        if (IsValid(IA_FireAxis))
        {
            Input->BindAction(IA_FireAxis, ETriggerEvent::Started, this, &ATankPawn::FireTower);
        }
    }
}

void ATankPawn::BeginPlay()
{
    Super::BeginPlay();

    PreviousLocation = GetActorLocation();

    // Initialize the previous position
    ReloadAmmo();
    SpawnLocation = GetActorLocation();
    OnAmmoDepleted.Broadcast(CurrentAmmo);

    //Health widget update
    if (IsValid(HealthWidgetComp))
    {
        UHealthBarWidgetBase* HealthBarWidget = Cast<UHealthBarWidgetBase>(HealthWidgetComp->GetWidget());
        if (IsValid(HealthBarWidget))
        {
            HealthBarWidget->SetHealthComponent(GetHealthComponent());
            HealthBarWidget->UpdateHealth(GetHealthComponent()->GetCurrentHealth());
        }
    }
}

void ATankPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    //Connects a play controller to the player
    const APlayerController* PC = Cast<APlayerController>(NewController);
    if (IsValid(PC))
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
        if (IsValid(Subsystem) && IsValid(IMC_InputContext))
        {
            Subsystem->AddMappingContext(IMC_InputContext, 1);
        }
    }
}

void ATankPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Speed calculation
    RealVelocity = (GetActorLocation() - PreviousLocation) / DeltaTime;
    PreviousLocation = GetActorLocation();

    // MoveTank
    const FVector InputVector = ConsumeMovementInputVector();
    const FVector TargetVelocity = !InputVector.IsNearlyZero() ? InputVector.GetSafeNormal() * Speed : FVector::ZeroVector;
    CurrentVelocity = FMath::VInterpTo(RealVelocity, TargetVelocity, DeltaTime, AccelerationSpeed);
    AddActorWorldOffset(CurrentVelocity * DeltaTime, true);

    // TurnTank
    AddActorWorldRotation(FRotator(0, RotInput * TurnTracksSpeed * DeltaTime, 0));

    // RotateTurret
    RotateTurret(GetControlRotation().Vector());

    // Programming Effects/Music
    HandleMoveEffects();
    HandleTurnEffects();

    // Reset inputs
    RotInput = 0.0f;
}

void ATankPawn::MoveTank(const FInputActionInstance& Value)
{
    const float InputValue = Value.GetValue().Get<float>();
    const FVector InputVector = GetActorForwardVector() * InputValue;
    AddMovementInput(InputVector);
}

void ATankPawn::HandleMoveEffects()
{
    const FVector InputVector;

    if (CurrentVelocity.Size() > StoppingInertia)
    {
        if (MoveSound && !MoveSound->IsPlaying())
        {
            MoveSound->Play();
        }
        if (CurrentMoveVFX && !CurrentMoveVFX->IsActive())
        {
            CurrentMoveVFX->Activate();
        }
    }
    else
    {
        if (MoveSound && MoveSound->IsPlaying())
        {
            MoveSound->FadeOut(FadeOutTime, 0);
        }
        if (CurrentMoveVFX && CurrentMoveVFX->IsActive())
        {
            CurrentMoveVFX->Deactivate();
        }
    }
}

void ATankPawn::TurnTank(const FInputActionInstance& Value)
{
    RotInput = Value.GetValue().Get<float>();
}

void ATankPawn::HandleTurnEffects()
{
    if (RotInput != 0.0f)
    {
        if (TurnSound && !TurnSound->IsPlaying())
        {
            TurnSound->Play();
        }
        if (CurrentTurnVFX && !CurrentTurnVFX->IsActive())
        {
            CurrentTurnVFX->Activate();
        }
    }
    else
    {
        if (TurnSound && TurnSound->IsPlaying())
        {
            TurnSound->FadeOut(FadeOutTime, 0);
        }
        if (CurrentTurnVFX && CurrentTurnVFX->IsActive())
        {
            CurrentTurnVFX->Deactivate();
        }
    }
}

void ATankPawn::TurnCamera(const FInputActionInstance& Value)
{
    const FVector2D MouseInput = Value.GetValue().Get<FVector2D>();
    AddControllerYawInput(MouseInput.X);
    AddControllerPitchInput(MouseInput.Y);
}
 
void ATankPawn::FireTower(const FInputActionInstance& Value)
{
    if (!bIsAlive || !IsValid(GetProjectileSpawnPoint()) || CurrentAmmo <= 0)
    {        
        FakeReload();
        return;
    }
    const bool BoolValue = Value.GetValue().Get<bool>();
    if (BoolValue && CanFire())
    {
        Fire();

        CurrentAmmo--;
        OnAmmoDepleted.Broadcast(CurrentAmmo);
    }
}
// Teleport to PlayerStart position, replenish ammunition
void ATankPawn::FakeReload()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
    if (PlayerStarts.Num() > 0)
    {
        SetActorLocation(PlayerStarts[0]->GetActorLocation());
    }
    ReloadAmmo();
    OnAmmoDepleted.Broadcast(CurrentAmmo);
}

void ATankPawn::ReloadAmmo()
{
    CurrentAmmo = MaxAmmo;

    UHealthComponent* HealthComponentMax = FindComponentByClass<UHealthComponent>();
    if (HealthComponentMax)
    {
        HealthComponentMax->RestoreToMaxHealth();
    }
}
