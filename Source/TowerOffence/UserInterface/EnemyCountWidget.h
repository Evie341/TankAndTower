#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TowerOffence/Actors/Player/MyPlayerController.h"
#include "BaseWidget.h"

#include "EnemyCountWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UEnemyCountWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetEnemyCount(int32 NewCount);

	virtual void NativeConstruct() override;

	virtual void InitializeForPlayer(AActor* PlayerActor) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnemyCountText;

	UPROPERTY(Transient)
	AMyPlayerController* PlayerController = nullptr;
};
