// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

#include "Components/BSStateComponent.h"
#include "Components/BSAttributeComponent.h"

void UBSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Character = Cast<ACharacter>(GetOwningActor()))
	{
		MovementComp = Character->GetCharacterMovement();
	}
}

void UBSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComp)
	{
		Velocity = MovementComp->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < 3.f && MovementComp->GetCurrentAcceleration() == FVector::ZeroVector;
		bIsFalling = MovementComp->IsFalling();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

		FRotator ControlRot = Character->GetControlRotation();
		FRotator PlayerRot = Character->GetActorRotation();
		FRotator DeltaRot = (ControlRot - PlayerRot).GetNormalized();

		AimYaw = DeltaRot.Yaw;
		AimPitch = DeltaRot.Pitch;
	}
}

void UBSAnimInstance::AnimNotify_ResetMovementInput()
{
	if (UBSStateComponent* StateComp = GetOwningActor()->GetComponentByClass<UBSStateComponent>())
	{
		StateComp->ToggleMovementInput(true);
	}
}

void UBSAnimInstance::AnimNotify_ResetState()
{
	if (UBSStateComponent* StateComp = GetOwningActor()->GetComponentByClass<UBSStateComponent>())
	{
		StateComp->ClearState();
	}
}

void UBSAnimInstance::AnimNotify_TogglePosture()
{
	if (UBSStateComponent* StateComp = GetOwningActor()->GetComponentByClass<UBSStateComponent>())
	{
		StateComp->ToggleMovementInput(true);
		StateComp->SetState(FGameplayTag::EmptyTag);
	}
}

void UBSAnimInstance::UpdateWeaponType(const EWeaponType& InWeaponType)
{
	WeaponType = InWeaponType;
}

void UBSAnimInstance::UpdateBlcokingState(const bool bBlockingState)
{
	bIsBlocking = bBlockingState;
}

void UBSAnimInstance::OnChangeCombat(const bool InCombatEnabled)
{
	bCombatEnabled = InCombatEnabled;
}
