#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::OnRegister()
{
	Super::OnRegister();

	CurrentHealth = MaxHealth;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		if (!Owner->OnTakeAnyDamage.IsAlreadyBound(this, &UHealthComponent::HandleTakeAnyDamage))
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	}
}

float UHealthComponent::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(FMath::FloorToInt(NewHealth), 0, FMath::FloorToInt(MaxHealth));
	OnHealthChanged.Broadcast(GetHealthPercent());
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	SetHealth(CurrentHealth - Damage);

	if (CurrentHealth <= 0)
	{
		OnDead.Broadcast();
	}
	if (AActor* DamagingActor = Cast<AActor>(DamageCauser))
	{
		LastDamageDirection = DamagingActor->GetVelocity().GetSafeNormal();
	}
}

void UHealthComponent::RestoreToMaxHealth()
{
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(GetHealthPercent());
}
