#include "MyHUD.h"

#include "TowerOffence/UserInterface/StartGameWidget.h"
#include "TowerOffence/UserInterface/EndGameWidget.h"
#include "TowerOffence/GameMode/GameModeBase/MyGameState.h" 
#include "TowerOffence/UserInterface/MenuWidget.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameMode.h"
#include "TowerOffence/Actors/Pawn/TankPawn.h"

void AMyHUD::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    CreateStatusWidget();

    if (AMyGameState* GameState = GetWorld()->GetGameState<AMyGameState>())
    {
        GameState->OnMatchStateHasBeenChanged.AddUniqueDynamic(this, &AMyHUD::HandleOnMatchStateChanged);
        HandleOnMatchStateChanged(NAME_None, GameState->GetMatchState());

        GameState->OnMatchStateChanged.AddDynamic(this, &AMyHUD::DisplayingStatusWidgetClass);
    }
    AController* Controller = GetOwningPlayerController();
    if (Controller)
    {
        Controller->OnPossessedPawnChanged.AddUniqueDynamic(this, &AMyHUD::HandleOnPawnChanged);
    }
}

void AMyHUD::ToggleMenu()
{
    if (MenuWidget == nullptr && IsValid(MenuWidgetClass))
    {
        MenuWidget = CreateWidget<UMenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
    }

    if (MenuWidget)
    {
        if (MenuWidget->IsInViewport())
        {
            MenuWidget->RemoveFromParent();
            GetOwningPlayerController()->bShowMouseCursor = false;
        }
        else
        {
            MenuWidget->AddToViewport();
            GetOwningPlayerController()->bShowMouseCursor = true;
        }
    }
}

void AMyHUD::HandleOnMatchStateChanged(const FName& OldMatchState, const FName& NewMatchState)
{
    if (NewMatchState == MatchState::WaitingToStart)
    {
        ShowStartGameWidget();
    }
    else if (NewMatchState == MatchState::InProgress)
    {
        HideStartGameWidget();
    }
}

void AMyHUD::CreateStatusWidget()
{
    StatusWidget = CreateWidget<UStatusWidget>(GetOwningPlayerController(), StatusWidgetClass);

    if (IsValid(StatusWidget))
    {
        StatusWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AMyHUD::DisplayingStatusWidgetClass()
{
    if (IsValid(StatusWidget))
    {
        StatusWidget->AddToViewport();
    }
}

void AMyHUD::AddWidgetToViewport(UUserWidget* Widget)
{
    if (IsValid(Widget))
    {
        Widget->AddToViewport();
    }
}

void AMyHUD::HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    ATankPawn* TankPawn = Cast<ATankPawn>(NewPawn);
    if (IsValid(StatusWidget) && TankPawn)
    {
        StatusWidget->SetVisibility(ESlateVisibility::Visible);
        StatusWidget->InitializeForPlayer(TankPawn);
    }
}

void AMyHUD::ShowEndGameWidget(bool bIsPlayerWinner)
{
    EndGameWidget = IsValid(EndGameWidgetClass) ? CreateWidget<UEndGameWidget>(GetWorld(), EndGameWidgetClass) : nullptr;
    if (IsValid(EndGameWidget))
    {
        EndGameWidget->Show(bIsPlayerWinner);
        EndGameWidget->AddToViewport();
    }
}

void AMyHUD::ShowStartGameWidget()
{
    StartGameWidget = IsValid(StartGameWidgetClass) ? CreateWidget<UStartGameWidget>(GetOwningPlayerController(), StartGameWidgetClass) : nullptr;
    if (IsValid(StartGameWidget))
    {
        StartGameWidget->AddToViewport();
    }
}

void AMyHUD::HideStartGameWidget()
{
    if (StartGameWidget)
    {
        StartGameWidget->RemoveFromParent();
    }
}
