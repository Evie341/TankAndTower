#include "AmmoWidget.h"

void UAmmoWidget::SetAmmoCount(int32 Ammo)
{
    if (RemainsAmmunition)
    {
        RemainsAmmunition->SetText(FText::AsNumber(Ammo));
    }
}

void UAmmoWidget::InitializeForPlayer(AActor* PlayerActor)
{
    ATankPawn* NewTankPawn = Cast<ATankPawn>(PlayerActor);
    if (NewTankPawn)
    {
        TankPawn = NewTankPawn;
        // Subscription to the number of cartridges
        TankPawn->OnAmmoDepleted.AddUniqueDynamic(this, &UAmmoWidget::SetAmmoCount);
        SetAmmoCount(TankPawn->GetCurrentAmmo());

        // Recharge subscription
        TankPawn->OnFire.AddUniqueDynamic(this, &UAmmoWidget::StartReloadAnimation);

        int32 MaxAmmo = TankPawn->GetMaxAmmo();
        if (GererakkyAmmunition)
        {
            GererakkyAmmunition->SetText(FText::AsNumber(MaxAmmo));
        }
    }
}

//Play reload animation.
void UAmmoWidget::StartReloadAnimation()
{
    PlayAnimation(RechargeAnimation);
}
