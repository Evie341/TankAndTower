#include "TurretPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "TowerOffence/Actors/Player/MyPlayerState.h"
#include "TowerOffence/Components/HealthComponent.h"

ATurretPawn::ATurretPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    BaseComponent = Cast<UPrimitiveComponent>(CreateDefaultSubobject(TEXT("BaseComponent"), UPrimitiveComponent::StaticClass(), USphereComponent::StaticClass(), true, false));
    SetRootComponent(BaseComponent);

    BoxTower = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxTower->SetupAttachment(BaseComponent);

    BoxBase = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleComponent"));
    BoxBase->SetupAttachment(BaseComponent);

    BaseMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(BoxTower);

    TurretMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("TurretMesh"));
    TurretMesh->SetupAttachment(BoxBase);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    // Aydio effects components
    MoveSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MoveSound"));
    MoveSound->SetupAttachment(BaseMesh);
    MoveSound->bAutoActivate = false;

    TurnSound = CreateDefaultSubobject<UAudioComponent>(TEXT("TurnSound"));
    TurnSound->SetupAttachment(BaseMesh);
    TurnSound->bAutoActivate = false;

    DeathSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathSound"));
    DeathSound->SetupAttachment(BaseMesh);
    DeathSound->bAutoActivate = false;

    StaticMoveSound = CreateDefaultSubobject<UAudioComponent>(TEXT("StaticMoveSound"));
    StaticMoveSound->SetupAttachment(BaseMesh);
    StaticMoveSound->bAutoActivate = false;

    TowerTurnSound = CreateDefaultSubobject<UAudioComponent>(TEXT("TowerTurnSound"));
    TowerTurnSound->SetupAttachment(TurretMesh);
    TowerTurnSound->bAutoActivate = false;

    FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSound"));
    FireSound->SetupAttachment(ProjectileSpawnPoint);
    FireSound->bAutoActivate = false;

    // Visual effects components
    CurrentMoveVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CurrentMoveVFX"));
    CurrentMoveVFX->SetupAttachment(BaseMesh);
    CurrentMoveVFX->bAutoActivate = false;

    CurrentTurnVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CurrentTurnVFX"));
    CurrentTurnVFX->SetupAttachment(BaseMesh);
    CurrentTurnVFX->bAutoActivate = false;

    CurrentTowerTurnVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CurrentTowerTurnVFX"));
    CurrentTowerTurnVFX->SetupAttachment(TurretMesh);
    CurrentTowerTurnVFX->bAutoActivate = false;

    DeathVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathVFX"));
    DeathVFX->SetupAttachment(BaseMesh);
    DeathVFX->bAutoActivate = false;

    StaticMoveVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StaticMoveVFX"));
    StaticMoveVFX->SetupAttachment(BaseMesh);
    StaticMoveVFX->bAutoActivate = false;

    FireVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireVFXComponent"));
    FireVFXComponent->SetupAttachment(ProjectileSpawnPoint);
    FireVFXComponent->bAutoActivate = false;

    if (BaseComponent)
    {
        BaseComponent->OnComponentHit.AddDynamic(this, &ATurretPawn::OnHit);
    }
}

void ATurretPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherComponent && (OtherComponent->Mobility == EComponentMobility::Static || OtherComponent->Mobility == EComponentMobility::Movable))
    {
        FVector RepelDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        OtherComponent->AddImpulse(RepelDirection * RepelForce, NAME_None, true);
    }
}

void ATurretPawn::OnConstruction(const FTransform& Transform) 
{
    Super::OnConstruction(Transform);

    DynamicMaterialBase = CreateAndSetDynamicMaterial(BaseMesh, BaseMeshMaterialSlot, BaseColorParameter);
    DynamicMaterialTurret = CreateAndSetDynamicMaterial(TurretMesh, TurretMeshMaterialSlot, TurretColorParameter);
}
 
void ATurretPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetVelocity().Size() > 0)
    {
        if (StaticMoveSound && StaticMoveSound->IsPlaying())
        {
            StaticMoveSound->Stop();
        }
        if (StaticMoveVFX && StaticMoveVFX->IsActive())
        {
            StaticMoveVFX->Deactivate();
        }
    }
    else
    {
        if (StaticMoveSound && !StaticMoveSound->IsPlaying())
        {
            StaticMoveSound->Play();
        }
        if (StaticMoveVFX && !StaticMoveVFX->IsActive())
        {
            StaticMoveVFX->Activate();
        }
    }
}

void ATurretPawn::RotateTurret(const FVector& TargetDirection)
{
    const FRotator RelativeRotation = TargetDirection.Rotation() - GetActorRotation();
    const FRotator CurrentRotation = GetTurretMesh()->GetRelativeRotation();
    const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, RelativeRotation.Yaw, CurrentRotation.Roll), GetWorld()->GetDeltaSeconds(), RotationSpeed);
    GetTurretMesh()->SetRelativeRotation(NewRotation);

    float CurrentYaw = FMath::UnwindDegrees(RelativeRotation.Yaw);
    float TargetYaw = FMath::UnwindDegrees(CurrentRotation.Yaw);
    CurrentYaw = FMath::Fmod(CurrentYaw + 360.0f, 360.0f);
    TargetYaw = FMath::Fmod(TargetYaw + 360.0f, 360.0f);
    float diff = FMath::Abs(CurrentYaw - TargetYaw);
    RotateTurretoggleEffects(diff >= 10.0f);
}

void ATurretPawn::RotateTurretoggleEffects(bool bShouldActivate)
{
    if (bShouldActivate)
    {
        if (TowerTurnSound && !TowerTurnSound->IsPlaying())
        {
            TowerTurnSound->Play();
        }
        if (CurrentTowerTurnVFX && !CurrentTowerTurnVFX->IsActive())
        {
            CurrentTowerTurnVFX->Activate();
        }
    }
    else
    {
        if (TowerTurnSound && TowerTurnSound->IsPlaying())
        {
            TowerTurnSound->FadeOut(FadeOutTime, 0);
        }
        if (CurrentTowerTurnVFX && CurrentTowerTurnVFX->IsActive())
        {
            CurrentTowerTurnVFX->Deactivate();
        }
    }
}
 
void ATurretPawn::HandleDeath()
{
    if (AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState()))
    {
        MyPlayerState->SetIsAlive(false);
    }

    OnPawnKilled.Broadcast(this);
    bIsAlive = false;

    EndGame(false);
    
    if (IsValid(BaseMesh))
    {
        BaseMesh->SetSimulatePhysics(true);
        BaseMesh->AddForce(LastHitDirection * ImpulseStrength, NAME_None, false);
    }
    if (IsValid(TurretMesh))
    {
        TurretMesh->SetSimulatePhysics(true);
        TurretMesh->AddForce(LastHitDirection * ImpulseStrength, NAME_None, false);
    }
    if (IsValid(BaseComponent))
    {
        BaseComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if (IsValid(BoxTower))
    {
        BoxTower->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if (IsValid(BoxBase))
    {
        BoxBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (DeathSound)
    {
        DeathSound->Play();
    }
    if (DeathVFX)
    {
        DeathVFX->Activate();
    }
    // Aydio effects components
    for (UAudioComponent* AudioComp : { MoveSound, TurnSound, TowerTurnSound, FireSound, StaticMoveSound })
    {
        if (AudioComp)
        {
            AudioComp->Stop();
        }
    }
    // Visual effects components
    for (UParticleSystemComponent* VFX : { CurrentMoveVFX, CurrentTurnVFX, CurrentTowerTurnVFX, FireVFXComponent, StaticMoveVFX })
    {
        if (VFX)
        {
            VFX->DeactivateSystem();
        }
    }
    //Camera shake
    if (DeathShake)
    {
        if (APlayerController* PC = GetController<APlayerController>())
        {
            PC->ClientStartCameraShake(DeathShake);
        }
    }
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ATurretPawn::StopDeathEffects, TimeDeathEffects, false);
}

void ATurretPawn::StopDeathEffects()
{
    if (DeathSound)
    {
        DeathSound->Stop();
    }
    if (DeathVFX)
    {
        DeathVFX->Deactivate();
    }
    Destroy();
}

UMaterialInstanceDynamic* ATurretPawn::CreateAndSetDynamicMaterial(UMeshComponent* Mesh, FName MaterialName, FName ColorParameter)
{
    if (!IsValid(Mesh) || MaterialName.IsNone() || ColorParameter.IsNone())
    {
        return nullptr;
    }

    // Знайти індекс матеріалу за його ім'ям
    int32 MaterialIndex = INDEX_NONE;
    const int32 NumMaterials = Mesh->GetNumMaterials();
    for (int32 Index = 0; Index < NumMaterials; ++Index)
    {
        UMaterialInterface* Material = Mesh->GetMaterial(Index);
        if (Material && Material->GetName() == MaterialName.ToString())
        {
            MaterialIndex = Index;
            break;
        }
    }

    if (MaterialIndex == INDEX_NONE)
    {
        return nullptr;
    }

    UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateDynamicMaterialInstance(MaterialIndex, Mesh->GetMaterial(MaterialIndex));
    if (IsValid(DynamicMaterial))
    {
        DynamicMaterial->SetVectorParameterValue(ColorParameter, TeamColor);
        Mesh->SetMaterial(MaterialIndex, DynamicMaterial);
    }
    return DynamicMaterial;
}

TArray<FString> ATurretPawn::GetBaseMeshMaterialSlotOptions() const
{
    return GetMaterialSlotOptions(BaseMesh);
}

TArray<FString> ATurretPawn::GetTurretMeshMaterialSlotOptions() const
{
    return GetMaterialSlotOptions(TurretMesh);
}

TArray<FString> ATurretPawn::GetBaseMeshColorParameterOptions() const
{
    return GetColorParameterOptions(BaseMesh, BaseMeshMaterialSlot);
}

TArray<FString> ATurretPawn::GetTurretMeshColorParameterOptions() const
{
    return GetColorParameterOptions(TurretMesh, TurretMeshMaterialSlot);
}

TArray<FString> ATurretPawn::GetMaterialSlotOptions(UGeometryCollectionComponent* GeometryComponent) const
{
    TArray<FString> MaterialSlots;
    if (!IsValid(GeometryComponent))
    {
        return MaterialSlots;
    }

    const int32 NumMaterials = GeometryComponent->GetNumMaterials();
    for (int32 Index = 0; Index < NumMaterials; ++Index)
    {
        UMaterialInterface* Material = GeometryComponent->GetMaterial(Index);
        if (Material)
        {
            MaterialSlots.Add(Material->GetName());
        }
    }
    return MaterialSlots;
}

TArray<FString> ATurretPawn::GetColorParameterOptions(UMeshComponent* MeshComponent, FName MaterialName) const
{
    TArray<FString> ColorParameters;

    if (!IsValid(MeshComponent))
    {
        return ColorParameters;
    }

    // Знайти індекс матеріалу за його ім'ям
    int32 MaterialIndex = INDEX_NONE;
    const int32 NumMaterials = MeshComponent->GetNumMaterials();
    for (int32 Index = 0; Index < NumMaterials; ++Index)
    {
        UMaterialInterface* Material = MeshComponent->GetMaterial(Index);
        if (Material && Material->GetName() == MaterialName.ToString())
        {
            MaterialIndex = Index;
            break;
        }
    }

    if (MaterialIndex == INDEX_NONE)
    {
        return ColorParameters;
    }

    UMaterialInterface* Material = MeshComponent->GetMaterial(MaterialIndex);
    if (!IsValid(Material))
    {
        return ColorParameters;
    }

    TArray<FMaterialParameterInfo> VectorParameters;
    TArray<FGuid> Guids;
    Material->GetAllVectorParameterInfo(VectorParameters, Guids);
    for (const FMaterialParameterInfo& Parameter : VectorParameters)
    {
        ColorParameters.Add(Parameter.Name.ToString());
    }
    return ColorParameters;  
}

void ATurretPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (IsValid(HealthComponent))
    {
        HealthComponent->OnDead.AddDynamic(this, &ATurretPawn::HandleDeath);
    }
}

void ATurretPawn::EndGame(bool bIsWinner)
{
    SetActorTickEnabled(false);
}

void ATurretPawn::Fire()
{
    if (!IsValid(GetProjectileSpawnPoint()))
    {
        return;
    }
    if (FireSound)
    {
        FireSound->Play();
    }
    if (FireVFXComponent)
    {
        FireVFXComponent->Activate(true);
    }
    //Camera shake
    if (FireShake)
    {
        if (APlayerController* PC = GetController<APlayerController>())
        {
            PC->ClientStartCameraShake(FireShake);
        }
    }
    const FVector SpawnLocation = GetProjectileSpawnPoint()->GetComponentLocation();
    const FRotator SpawnRotation = GetTurretMesh()->GetComponentRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    OnFire.Broadcast();

    GetWorld()->SpawnActor<AActor>(SpawnProjectileBlueprint, SpawnLocation, SpawnRotation, SpawnParams);
    LastFireTime = GetWorld()->GetTimeSeconds();
}

bool ATurretPawn::CanFire() const
{
    if (!bIsAlive || !IsValid(GetProjectileSpawnPoint()))
    {
        return false;
    }
    return GetWorld()->GetTimeSeconds() - LastFireTime >= ShotInterval;
}
 