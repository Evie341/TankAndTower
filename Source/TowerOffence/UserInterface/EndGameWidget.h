#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EndGameWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(bool bIsPlayerWinner);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoseText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton = nullptr;

	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> MainMenuLevel = nullptr;

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
}; 
