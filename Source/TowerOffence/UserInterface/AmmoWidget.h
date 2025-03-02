#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "TowerOffence/Actors/Pawn/TankPawn.h"
#include "BaseWidget.h"
#include "AmmoWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UAmmoWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void SetAmmoCount(int32 CurrentAmmo);
	
	UFUNCTION()
	void StartReloadAnimation();

	virtual void InitializeForPlayer(AActor* PlayerActor) override;

private:
	ATankPawn* TankPawn = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RechargeAnimation;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RechargeTime = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GererakkyAmmunition = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainsAmmunition = nullptr;
};
