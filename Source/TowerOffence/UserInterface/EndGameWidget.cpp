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
