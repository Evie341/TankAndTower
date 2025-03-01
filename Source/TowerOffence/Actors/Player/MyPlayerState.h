#pragma once

#include "GameFramework/PlayerState.h"
#include "TowerOffence/Actors/Pawn/TeamInfo.h"

#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS()
class TOWEROFFENCE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Team")
    ATeamInfo* GetTeamInfo() const;

    UFUNCTION(BlueprintCallable, Category = "Team")
    void SetTeamInfo(ATeamInfo* NewTeamInfo);

    UFUNCTION(BlueprintCallable, Category = "State")
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "State")
    void SetIsAlive(bool bNewIsAlive);

    UPROPERTY(BlueprintAssignable)
    FOnDeathDelegate OnDeath;

private:
    UPROPERTY(VisibleAnywhere, Category = "Team")
    ATeamInfo* TeamInfo = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "State")
    bool bIsAlive = true;
};
