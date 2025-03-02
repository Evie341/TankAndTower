#include "MyPlayerController.h"

#include "TowerOffence/Actors/Pawn/TurretPawn.h"
#include "TowerOffence/UserInterface/MyHUD.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameState.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameModeBase.h"

void AMyPlayerController::EndGame(bool bIsWinner)
{
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());
	if (IsValid(HUD))
	{
		HUD->ShowEndGameWidget(bIsWinner);
	}
	ATurretPawn* TurretPawn = Cast<ATurretPawn>(GetPawn());
	if (IsValid(TurretPawn))
	{
		TurretPawn->EndGame(bIsWinner);
	}
}

int32 AMyPlayerController::GetTeamId() const
{
	AMyPlayerState* CurrentPlayerState = Cast<AMyPlayerState>(PlayerState);
	return IsValid(CurrentPlayerState) && IsValid(CurrentPlayerState->GetTeamInfo()) ? CurrentPlayerState->GetTeamInfo()->GetUniqueID() : INDEX_NONE;
}

void AMyPlayerController::HandleOnDeath()
{
	if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
	{
		AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
		if (MyPlayerState && MyPlayerState->GetTeamInfo())
		{
			int32 EnemyCount = MyGameState->GetAliveEnemyCount(MyPlayerState->GetTeamInfo()->GetUniqueID());
			OnEnemyCountChanged.Broadcast(EnemyCount);
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMyPlayerController::ToggleHUDMenu);
}

void AMyPlayerController::ToggleHUDMenu()
{
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());
	if (HUD)
	{
		HUD->ToggleMenu();
	}
}
