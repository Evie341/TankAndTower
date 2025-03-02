#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TowerOffence/Actors/Pawn/TeamInfo.h"
 
#include "MyGameModeBase.generated.h"

UCLASS()
class TOWEROFFENCE_API AMyGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	AMyGameModeBase();

	virtual void HandleMatchIsWaitingToStart() override;

	const TArray<ATeamInfo*>& GetTeams() const { return Teams; }

private:
	virtual bool ReadyToStartMatch_Implementation() override;
	
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;//

	virtual void RestartGame()override;
	virtual void StartPlay() override;

	void SpawnTeams();

	UFUNCTION()
	void UpdateEnemyCount();

	UFUNCTION()
	void PawnKilled(APawn* PawnKilled);

	UFUNCTION()
	void EndGame(int WinningTeamID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "Tower"))
	float RestartDelay = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Teams")
	int32 NumberOfTeams = 2;

	TArray<int32> GetAliveTeams() const;
	
	FTimerHandle RestartTimer;

	UPROPERTY()
	TArray<ATeamInfo*> Teams;
};
