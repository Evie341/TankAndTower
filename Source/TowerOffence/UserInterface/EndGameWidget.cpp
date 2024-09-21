#include "EndGameWidget.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

void UEndGameWidget::Show(bool bIsPlayerWinner)
{
    if (IsValid(WinText))
    {
        WinText->SetVisibility(bIsPlayerWinner ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    if (IsValid(LoseText))
    {
        LoseText->SetVisibility(bIsPlayerWinner ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
    if (IsValid(RestartButton))
    {
        RestartButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnRestartButtonClicked);
    }
    if (IsValid(ExitButton))
    {
        ExitButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnExitButtonClicked);
    }

    AddToViewport();

    APlayerController* PlayerController = GetOwningPlayer();
    if (IsValid(PlayerController))
    {
        PlayerController->bShowMouseCursor = true;
    }
}

void UEndGameWidget::OnRestartButtonClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (IsValid(PlayerController))
    {
        PlayerController->bShowMouseCursor = false;
    }
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UEndGameWidget::OnExitButtonClicked()
{
    if (MainMenuLevel.IsNull()) return;

    FString LevelName = MainMenuLevel.ToSoftObjectPath().GetAssetName();
    UGameplayStatics::OpenLevel(this, FName(*LevelName));
}
