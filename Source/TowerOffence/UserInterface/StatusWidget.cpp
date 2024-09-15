#include "StatusWidget.h"

#include "HealthBarWidgetBase.h" 
#include "EnemyCountWidget.h" 
#include "AmmoWidget.h"  

void UStatusWidget::InitializeForPlayer(AActor* PlayerActor)
{
    TArray<UBaseWidget*> Widgets = { HealthBarWidget, AmmoWidget, EnemyCountWidget };

    for (UBaseWidget* Widget : Widgets)
    {
        if (Widget && PlayerActor)
        {
            Widget->InitializeForPlayer(PlayerActor);
        }
    }
}
