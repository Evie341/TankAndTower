#include "MyGameModeBase.h"

#include "TowerOffence/Actors/Pawn/TankPawn.h"
#include "TowerOffence/UserInterface/MyHUD.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameState.h"
#include "TowerOffence/Actors/Player/MyPlayerController.h"
#include "TowerOffence/Actors/Player/MyPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameSession.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = ATankPawn::StaticClass();
	HUDClass = AMyHUD::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}

// Team assignment
void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnTeams();

	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (AMyPlayerState* PlayerState = It->Get()->GetPlayerState<AMyPlayerState>())
		{
			int32 TeamIndex = PlayerState->IsABot() ? 1 : 0;
			if (Teams.IsValidIndex(TeamIndex))
			{
				Teams[TeamIndex]->AddPlayer(PlayerState);
				PlayerState->SetTeamInfo(Teams[TeamIndex]);
			}
		}
		if (ATurretPawn* Pawn = Cast<ATurretPawn>(It->Get()->GetPawn()))
		{
			Pawn->OnPawnKilled.AddUniqueDynamic(this, &AMyGameModeBase::PawnKilled);
		}
	}
}

// Create ATeamInfo for each team
void AMyGameModeBase::SpawnTeams()
{
	for (int32 i = 0; i < NumberOfTeams; ++i)
	{
		ATeamInfo* NewTeam = GetWorld()->SpawnActor<ATeamInfo>();
		if (NewTeam)
		{
			Teams.Add(NewTeam);
		}
	}
}

APawn* AMyGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* SpawnedPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	if (ATurretPawn* TurretPawn = Cast<ATurretPawn>(SpawnedPawn))
	{
		TurretPawn->OnPawnKilled.AddUniqueDynamic(this, &AMyGameModeBase::PawnKilled);
	}
	return SpawnedPawn;
}

void AMyGameModeBase::HandleMatchIsWaitingToStart()
{
	if (GameSession != nullptr)
	{
		GameSession->HandleMatchIsWaitingToStart();
	}
	UE_LOG(LogTemp, Display, TEXT("Waiting to start..."));
}

bool AMyGameModeBase::ReadyToStartMatch_Implementation()
{
	if (AMyGameState* MyGameState = GetGameState<AMyGameState>())
	{
		return MyGameState->IsMatchReadyToStart();
	}
	return false;
}

void AMyGameModeBase::PawnKilled(APawn* PawnKilled)
{
	const TArray<int32> AliveTeams = GetAliveTeams();
	if (AliveTeams.Num() == 1)
	{
		const int WinningTeamID = AliveTeams[0];
		EndGame(WinningTeamID);
	}
	UpdateEnemyCount();
}

//Updates the number of enemies in the game for each valid player controller.
void AMyGameModeBase::UpdateEnemyCount()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(*It);
		if (IsValid(PlayerController))
		{
			PlayerController->HandleOnDeath();
		}
	}
}

void AMyGameModeBase::EndGame(int WinningTeamID)
{
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(*It);
		if (IsValid(PlayerController))
		{
			const int32 PlayerTeamID = PlayerController->GetTeamId();
			const bool bIsPlayerWinner = PlayerTeamID == WinningTeamID;
			PlayerController->EndGame(bIsPlayerWinner);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &AMyGameModeBase::RestartGame, RestartDelay);
}

void AMyGameModeBase::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

TArray<int32> AMyGameModeBase::GetAliveTeams() const
{
	TSet<int32> AliveTeams;

	for (ATeamInfo* Team : Teams)
	{
		if (!IsValid(Team)) continue;

		for (APlayerState* PlayerState : Team->TeamPlayers)
		{
			const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState);
			if (IsValid(MyPlayerState) && MyPlayerState->IsAlive())
			{
				AliveTeams.Add(Team->GetUniqueID());
				break; 
			}
		}
	}
	return AliveTeams.Array();
}
