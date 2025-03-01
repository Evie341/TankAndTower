#include "StartGameWidget.h"
#include "TowerOffence/Actors/Pawn/TurretPawn.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameState.h"
#include "GameFramework/GameSession.h"

void UStartGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (AMyGameState* GameState = GetWorld()->GetGameState<AMyGameState>())
    {
        GameState->OnTimerTick.AddUniqueDynamic(this, &UStartGameWidget::HandleOnTimerTick);
        HandleOnTimerTick(GameState->GetTimeRemaining());
    }
}

void UStartGameWidget::HandleOnTimerTick(int32 SecondsToStartMatch)
{
    if (IsValid(StartGame))
    {
        StartGame->SetText(FText::AsNumber((SecondsToStartMatch)));
    }
}
