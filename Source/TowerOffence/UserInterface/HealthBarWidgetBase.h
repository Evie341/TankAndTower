#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "TowerOffence/Actors/Pawn/TankPawn.h"
#include "BaseWidget.h"

#include "HealthBarWidgetBase.generated.h"

UCLASS()
class TOWEROFFENCE_API UHealthBarWidgetBase : public UBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float Health);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthComponent(UHealthComponent* NewHealthComponent);

	virtual void InitializeForPlayer(AActor* PlayerActor) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthText = nullptr;

	UPROPERTY(Transient)
	UHealthComponent* HealthComponent = nullptr;
};
