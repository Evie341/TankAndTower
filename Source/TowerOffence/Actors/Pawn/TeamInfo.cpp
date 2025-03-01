#include "TeamInfo.h"

#include "TowerOffence/Actors/Player/MyPlayerState.h"

ATeamInfo::ATeamInfo()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATeamInfo::AddPlayer(APlayerState* Player)
{
    if (Player)
    {
        TeamPlayers.Add(Player);
    }
}

int32 ATeamInfo::GetAlivePlayerCount() const
{
    int32 AliveCount = 0;
    for (APlayerState* PlayerState : TeamPlayers)
    {
        const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState);
        if (IsValid(MyPlayerState) && MyPlayerState->IsAlive())
        {
            AliveCount++;
        }
    }
    return AliveCount;
}
