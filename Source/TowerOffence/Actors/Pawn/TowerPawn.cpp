#include "TowerPawn.h"

#include "TankPawn.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "TowerOffence/Components/HealthComponent.h"
#include "TowerOffence/UserInterface/HealthBarWidgetBase.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameModeBase.h"

ATowerPawn::ATowerPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass(TEXT("BaseComponent"), UCapsuleComponent::StaticClass()))
{
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetCollisionProfileName(TEXT("Trigger"));
	DetectionSphere->SetupAttachment(RootComponent);

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->SetupAttachment(RootComponent);
}
 
void ATowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* TargetPawn = GetClosestTarget();
	if (TargetPawn)
	{
		const FVector TargetActorLocation = TargetPawn->GetActorLocation();
		const FVector Direction = (TargetActorLocation - GetActorLocation()).GetSafeNormal();
		RotateTurret(Direction);

		const float DirectionYaw = Direction.Rotation().Yaw;
		const float ForwardYaw = GetTurretMesh()->GetRelativeRotation().Yaw;
		const float DeltaYaw = FMath::FindDeltaAngleDegrees(DirectionYaw, ForwardYaw);

		if (FMath::Abs(DeltaYaw) < YawTolerance && CanFire())
		{
			Fire();
		}
	}
}

void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthWidgetComp))
	{
		UHealthBarWidgetBase* HealthBarWidget = Cast<UHealthBarWidgetBase>(HealthWidgetComp->GetWidget());
		if (HealthBarWidget != nullptr)
		{
			HealthBarWidget->SetHealthComponent(GetHealthComponent());
		}
	}
}

APawn* ATowerPawn::GetClosestTarget() const
{
	APawn* ClosestPawn = nullptr;
	float ClosestDistanceSq = FLT_MAX;

	for (AActor* TargetActor : TargetActors)
	{
		APawn* TargetPawn = Cast<APawn>(TargetActor);
		if (!TargetPawn) return nullptr;

		AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(TargetPawn->GetPlayerState());
		if (!MyPlayerState || !MyPlayerState->IsAlive()) return nullptr;

		const float DistanceSq = FVector::DistSquared(TargetPawn->GetActorLocation(), GetActorLocation());
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			ClosestPawn = TargetPawn;
		}
	}
	return ClosestPawn;
}


void ATowerPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(DetectionSphere))
	{
		DetectionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATowerPawn::OnOverlapBegin);
		DetectionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ATowerPawn::OnOverlapEnd);
	}
}

void ATowerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(Cast<ATankPawn>(OtherActor)))
	{
		TargetActors.AddUnique(Cast<APawn>(OtherActor));
	}
}

void ATowerPawn::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(Cast<ATankPawn>(OtherActor)))
	{
		TargetActors.RemoveSwap(Cast<APawn>(OtherActor));
	}
}
