// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSAttributeComponent.h"

#include "Components/BSStateComponent.h"
#include "BSGameplayTag.h"

UBSAttributeComponent::UBSAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBSAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBSAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UBSAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UBSAttributeComponent::ToggleStaminaRegen(bool bEnabled, float DelayTime)
{
	if (bEnabled)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &ThisClass::RegenrateStaminaHandle, 0.1f, true, DelayTime);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
}

void UBSAttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);

	BroadcastAttributeChanged(EAttributeType::Stamina);
}

void UBSAttributeComponent::BroadcastAttributeChanged(const EAttributeType& InAttributeType) const
{
	float Ratio = 0.f;

	switch (InAttributeType)
	{
	case EAttributeType::Stamina:
		Ratio = GetStaminaRatio();
		break;
	case EAttributeType::Health:
		Ratio = GetHealthRatio();
		break;
	}

	OnChangedAttribute.Broadcast(InAttributeType, Ratio);
}

void UBSAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.f, MaxHealth);

	if (BaseHealth <= 0.f)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		if (UBSStateComponent* StateComp = GetOwner()->GetComponentByClass<UBSStateComponent>())
		{
			StateComp->SetState(BSGameplayTag::Character_State_Death);
		}
	}

	BroadcastAttributeChanged(EAttributeType::Health);
}

void UBSAttributeComponent::RegenrateStaminaHandle()
{
	BaseStamina = FMath::Clamp(BaseStamina + RegenStaminaRate, 0.f, MaxStamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegen(false);
	}

	BroadcastAttributeChanged(EAttributeType::Stamina);
}

