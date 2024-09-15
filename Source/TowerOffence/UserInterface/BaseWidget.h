#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

UCLASS()
class TOWEROFFENCE_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	virtual void InitializeForPlayer(AActor* PlayerActor);
};
