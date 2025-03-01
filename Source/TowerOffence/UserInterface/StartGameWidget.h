#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "StartGameWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UStartGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartGame = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnTimerTick(int32 SecondsToStartMatch);
};
