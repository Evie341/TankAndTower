#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuWidget.h"
#include "MyMenuHUD.generated.h"

UCLASS()
class TOWEROFFENCE_API AMyMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass = nullptr;

	UPROPERTY(Transient)
	class UMainMenuWidget* MainMenuWidget = nullptr;
};
