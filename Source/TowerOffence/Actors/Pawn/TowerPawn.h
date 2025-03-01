#pragma once

#include "TurretPawn.h"
#include "Components/DrawSphereComponent.h"
#include "Components/WidgetComponent.h" 
#include "TowerPawn.generated.h"

UCLASS()
class TOWEROFFENCE_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
	ATowerPawn(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidgetComp;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DetectionSphere", meta = (AllowPrivateAccess = "true"))
	USphereComponent* DetectionSphere = nullptr;

	UPROPERTY(Transient)
	TArray<APawn*> TargetActors;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	APawn* GetClosestTarget() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "Tower"))
	float YawTolerance = 0.01f;
};
