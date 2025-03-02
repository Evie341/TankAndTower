#include "MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UMenuWidget::Initialize()
{
    bool bIsInitialized = Super::Initialize();
    if (!bIsInitialized)
    {
        return false;
    }
    if (IsValid(StartButton))
    {
        StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartButtonClicked);
    }
    if (IsValid(ExitButton))
    {
        ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitButtonClicked);
    }

    return true;
}

void UMenuWidget::OnStartButtonClicked()
{
    if (GameLevel.IsNull())
    {
        return;
    }
    FString LevelName = GameLevel.ToSoftObjectPath().GetAssetName();
    UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void UMenuWidget::OnExitButtonClicked()
{
    if (UWorld* World = GetWorld())
    {
        UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
    }
}
