#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerOffence/UserInterface/HealthBarWidgetBase.h"
#include "TowerOffence/UserInterface/StatusWidget.h"

#include "MyHUD.generated.h"

UCLASS()
class TOWEROFFENCE_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void AddWidgetToViewport(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void ToggleMenu();

	UFUNCTION()
	void HandleOnMatchStateChanged(const FName& OldMatchState, const FName& NewMatchState);
	
	UFUNCTION()
	void ShowEndGameWidget(bool bIsPlayerWinner);

	UFUNCTION()
	void ShowStartGameWidget();

	UFUNCTION()
	void HideStartGameWidget();

	UPROPERTY(Transient)
	class UStatusWidget* StatusWidget = nullptr;

private:
	UFUNCTION()
	void HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void CreateStatusWidget();

	UFUNCTION()
	void DisplayingStatusWidgetClass();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UStatusWidget> StatusWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UEndGameWidget> EndGameWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UStartGameWidget> StartGameWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMenuWidget> MenuWidgetClass = nullptr;

	UPROPERTY(Transient)
	class UStartGameWidget* StartGameWidget = nullptr;

	UPROPERTY(Transient)
	class UEndGameWidget* EndGameWidget = nullptr;

	UPROPERTY(Transient)
	class UMenuWidget* MenuWidget = nullptr;
};
