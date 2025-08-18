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
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &ThisClass::RegenerateStaminaHandle, 0.1f, true, DelayTime);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
}

void UBSAttributeComponent::TogglePostureRegen(bool bEnabled, float DelayTime)
{
	if (bEnabled)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(PostureRegenTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(PostureRegenTimer, this, &ThisClass::RegeneratePostureHandle, 0.1f, true, DelayTime);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PostureRegenTimer);
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
	case EAttributeType::Posture:
		Ratio = GetPostureRatio();
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

void UBSAttributeComponent::TakePostureAmount(float DamageAmount)
{
	float ActualPosture = DamageAmount / WeightPosture;

	BasePosture = FMath::Clamp(BasePosture + ActualPosture, 0.f, MaxPosture);

	if (BasePosture >= MaxPosture)
	{
		if (UBSStateComponent* StateComp = GetOwner()->GetComponentByClass<UBSStateComponent>())
		{
			FGameplayTagContainer CheckTags;
			CheckTags.AddTag(BSGameplayTag::Character_State_Death);

			if (StateComp->IsCurrentStateEqualToAny(CheckTags) == false)
			{
				// 체간 게이지가 다 찼을 때 행동
				if (OnMaxPosture.IsBound())
				{
					OnMaxPosture.Broadcast();
				}
			}
		}
	}

	BroadcastAttributeChanged(EAttributeType::Posture);
}

void UBSAttributeComponent::ResetAttribute()
{
	BaseHealth = MaxHealth;
	BaseStamina = MaxStamina;

	BroadcastAttributeChanged(EAttributeType::Health);
	BroadcastAttributeChanged(EAttributeType::Stamina);
}

void UBSAttributeComponent::RegenerateStaminaHandle()
{
	BaseStamina = FMath::Clamp(BaseStamina + RegenStaminaRate, 0.f, MaxStamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegen(false);
	}

	BroadcastAttributeChanged(EAttributeType::Stamina);
}

void UBSAttributeComponent::RegeneratePostureHandle()
{
	BasePosture = FMath::Clamp(BasePosture - RegenPostureRate, 0.f, MaxPosture);

	if (BasePosture <= 0.f)
	{
		TogglePostureRegen(false);
	}

	BroadcastAttributeChanged(EAttributeType::Posture);
}