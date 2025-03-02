#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerState.h"
#include "MyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChangedSignature, int32, NewEnemyCount);

UCLASS()
class TOWEROFFENCE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnemyCountChangedSignature OnEnemyCountChanged;

	UFUNCTION()
	void EndGame(bool bIsWinner);

	UFUNCTION()
	void HandleOnDeath();

	int32 GetTeamId() const;

	void ToggleHUDMenu();
};
