// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_BSAirAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_BSAirAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
			{
				MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);
			}
		}
	}
}

void UAnimNotifyState_BSAirAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
			{
				MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		}
	}
}
