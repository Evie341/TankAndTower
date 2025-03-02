#include "EnemyCountWidget.h"

#include "TowerOffence/GameMode/GameModeBase/MyGameState.h"
#include "TowerOffence/Actors/Player/MyPlayerController.h"
#include "StatusWidget.h"

void UEnemyCountWidget::NativeConstruct()
{
	Super::NativeConstruct();
 
}

void UEnemyCountWidget::SetEnemyCount(int32 NewCount)
{
	if (EnemyCountText)
	{
		EnemyCountText->SetText(FText::AsNumber(NewCount));
	}
}

void UEnemyCountWidget::InitializeForPlayer(AActor* PlayerActor)
{
	APawn* Pawn = Cast<APawn>(PlayerActor);
	if (IsValid(Pawn))
	{
		AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(MyPlayerController))
		{
			MyPlayerController->OnEnemyCountChanged.AddDynamic(this, &UEnemyCountWidget::SetEnemyCount);
			if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
			{
				SetEnemyCount(MyGameState->GetAliveEnemyCount(MyPlayerController->GetTeamId()));
			}

			AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(MyPlayerController->PlayerState);
			if (IsValid(MyPlayerState))
			{
				MyPlayerState->OnDeath.AddUniqueDynamic(MyPlayerController, &AMyPlayerController::HandleOnDeath);
			}
		}
	}
}
