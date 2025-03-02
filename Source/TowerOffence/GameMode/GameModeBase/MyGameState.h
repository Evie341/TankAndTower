#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TowerOffence/Actors/Player/MyPlayerState.h" 
#include "MyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerTickSignature, int32, SecondsRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchStateHasBeenChangedSignature, const FName&, OldMatchState, const FName&, NewMatchState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStateChanged);

UCLASS()
class TOWEROFFENCE_API AMyGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMyGameState();

	UFUNCTION(BlueprintPure, Category = "Game")
	float GetTimeRemaining() const { return StartDelay; }

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void OnRep_MatchState() override;

	int32 GetAliveEnemyCount(int32 TeamID) const;

	bool IsMatchReadyToStart() const;

	UPROPERTY(BlueprintAssignable)
	FOnTimerTickSignature OnTimerTick;

	UPROPERTY(BlueprintAssignable)
	FOnMatchStateHasBeenChangedSignature OnMatchStateHasBeenChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMatchStateChanged OnMatchStateChanged;

private:
	bool bIsMatchReadyToStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (AllowPrivateAccess = "true"))
	int StartDelay = 5.0f;

	float TimeRemaining = 0.0f;

	UFUNCTION()
	void UpdateCountdown();

	void SetReadyToStart();

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	TArray<AMyPlayerState*> PlayerStates;
};
