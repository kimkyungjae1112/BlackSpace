// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBSStateComponent::UBSStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBSStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBSStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBSStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UBSStateComponent::ClearState()
{
	if (CurrentState == BSGameplayTag::Character_State_Death) return;

	CurrentState = FGameplayTag::EmptyTag;
}

bool UBSStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}

bool UBSStateComponent::IsCurrentStateEqualToIt(const FGameplayTag& TagToCheck) const
{
	return CurrentState == TagToCheck;
}

void UBSStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();
}