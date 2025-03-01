#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerOffence/Actors/Pawn/TankPawn.h"
#include "BaseWidget.h"

#include "StatusWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChanged);

UCLASS()
class TOWEROFFENCE_API UStatusWidget : public UBaseWidget
{
	GENERATED_BODY()
 
public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStatusChanged OnStatusChanged;

    virtual void InitializeForPlayer(AActor* PlayerActor) override;

private:
    UPROPERTY(meta = (BindWidget))
    class UHealthBarWidgetBase* HealthBarWidget = nullptr;

    UPROPERTY(meta = (BindWidget))
    class UEnemyCountWidget* EnemyCountWidget = nullptr;

    UPROPERTY(meta = (BindWidget))
    class UAmmoWidget* AmmoWidget = nullptr;
};
