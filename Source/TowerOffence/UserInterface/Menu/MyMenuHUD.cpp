#include "MyMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AMyMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	APlayerController* PlayerController = IsValid(MainMenuWidgetClass) ? GetWorld()->GetFirstPlayerController() : nullptr;
	MainMenuWidget = IsValid(PlayerController) ? CreateWidget<UMainMenuWidget>(PlayerController, MainMenuWidgetClass) : nullptr;

	if (IsValid(MainMenuWidget))
	{
		MainMenuWidget->AddToViewport();
	}
}
