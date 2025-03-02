#include "MyGameState.h"

#include "TimerManager.h"
#include "TowerOffence/UserInterface/StartGameWidget.h"
#include "TowerOffence/UserInterface/MyHUD.h"

#include "MyGameModeBase.h"
 
AMyGameState::AMyGameState()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyGameState::SetReadyToStart()
{
    bIsMatchReadyToStart = true;
}

bool AMyGameState::IsMatchReadyToStart() const
{
    return bIsMatchReadyToStart;
}

void AMyGameState::HandleMatchIsWaitingToStart()
{
    Super::HandleMatchIsWaitingToStart();
    TimeRemaining = StartDelay;
    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameState::UpdateCountdown, 1.0f, true);
}

void AMyGameState::OnRep_MatchState()
{
    OnMatchStateHasBeenChanged.Broadcast(PreviousMatchState, MatchState);

    OnMatchStateChanged.Broadcast();

    Super::OnRep_MatchState();
}

void AMyGameState::UpdateCountdown()
{
    if (TimeRemaining <= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
        bIsMatchReadyToStart = true;
        return;
    }
    TimeRemaining -= 1;
    OnTimerTick.Broadcast(TimeRemaining);
}
 
int32 AMyGameState::GetAliveEnemyCount(int32 TeamID) const
{
    int32 EnemyCount = 0;

    if (const AMyGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AMyGameModeBase>())
    {
        const TArray<ATeamInfo*>& Teams = GameMode->GetTeams();

        for (ATeamInfo* Team : Teams)
        {
            if (IsValid(Team) && Team->GetUniqueID() != TeamID)
            {
                EnemyCount += Team->GetAlivePlayerCount();
            }
        }
    }
    return EnemyCount;
}
