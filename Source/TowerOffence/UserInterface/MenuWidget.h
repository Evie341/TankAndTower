#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton = nullptr;

    UPROPERTY(EditAnywhere, Category = "Level")
    TSoftObjectPtr<UWorld> GameLevel = nullptr;

    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnExitButtonClicked();
};
