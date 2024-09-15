#include "MyPlayerState.h"

#include "TowerOffence/GameMode/GameModeBase/MyGameState.h"

ATeamInfo* AMyPlayerState::GetTeamInfo() const
{
    return TeamInfo;
}

void AMyPlayerState::SetTeamInfo(ATeamInfo* NewTeamInfo)
{
    TeamInfo = NewTeamInfo;
}

bool AMyPlayerState::IsAlive() const
{
    return bIsAlive;
}

void AMyPlayerState::SetIsAlive(bool bNewIsAlive)
{
    bIsAlive = bNewIsAlive;

    if (!bIsAlive)
    {
        OnDeath.Broadcast();
    }
}
