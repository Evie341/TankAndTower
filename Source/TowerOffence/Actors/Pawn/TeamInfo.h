#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamInfo.generated.h"

UCLASS()
class TOWEROFFENCE_API ATeamInfo : public AInfo
{
	GENERATED_BODY()
	
public:
    ATeamInfo();

    void AddPlayer(APlayerState* Player);

	int32 GetAlivePlayerCount() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	TArray<APlayerState*> TeamPlayers;
};

