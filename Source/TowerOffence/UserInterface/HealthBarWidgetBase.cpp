#include "HealthBarWidgetBase.h"

#include "TowerOffence/Components/HealthComponent.h"

void UHealthBarWidgetBase::UpdateHealth(float Health)
{
    if (IsValid(HealthComponent))
    {
        float CurrentHealth = HealthComponent->GetCurrentHealth();
        float MaxHealth = HealthComponent->GetMaxHealth();
        float HealthPercent = HealthComponent->GetHealthPercent();

        if (IsValid(CurrentHealthText))
        {
            CurrentHealthText->SetText(FText::AsNumber(CurrentHealth));
        }
        if (IsValid(MaxHealthText))
        {
            MaxHealthText->SetText(FText::AsNumber(MaxHealth));
        }
        if (IsValid(HealthProgressBar))
        {
            HealthProgressBar->SetPercent(HealthPercent);
        }
    }
}

void UHealthBarWidgetBase::SetHealthComponent(UHealthComponent* NewHealthComponent)
{
    if (IsValid(HealthComponent))
    {
        HealthComponent->OnHealthChanged.RemoveDynamic(this, &UHealthBarWidgetBase::UpdateHealth);
    }
    if (IsValid(NewHealthComponent))
    {
        HealthComponent = NewHealthComponent;
        HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &UHealthBarWidgetBase::UpdateHealth);
        UpdateHealth(HealthComponent->GetCurrentHealth());
    }
}

void UHealthBarWidgetBase::InitializeForPlayer(AActor* PlayerActor)
{
    ATankPawn* TankPawn = Cast<ATankPawn>(PlayerActor);
    if (TankPawn)
    {
        UHealthComponent* HealthComp = TankPawn->GetHealthComponent();
        SetHealthComponent(HealthComp);
    }
}
