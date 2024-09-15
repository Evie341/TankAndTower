#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
    bool bIsInitialized = Super::Initialize();
    if (!bIsInitialized) return false;

    if (IsValid(StartButton))
    {
        StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
    }
    if (IsValid(ExitButton))
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
    }

    APlayerController* PlayerController = GetOwningPlayer();
    if (IsValid(PlayerController))
    {
        PlayerController->bShowMouseCursor = true;
    }

    return true;
}

void UMainMenuWidget::OnStartButtonClicked() 
{
    if (GameLevel.IsNull()) return;

    FString LevelName = GameLevel.ToSoftObjectPath().GetAssetName();
    UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void UMainMenuWidget::OnExitButtonClicked() 
{
    if (UWorld* World = GetWorld())
    {
        UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
    }
}
