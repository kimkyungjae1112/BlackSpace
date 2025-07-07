// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_BSRotateToTarget.h"

#include "Components/BSRotationComponent.h"

void UAnimNotifyState_BSRotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UBSRotationComponent* RotationComp = MeshComp->GetOwner()->GetComponentByClass<UBSRotationComponent>())
	{
		RotationComp->ToggleRotation(true);
	}
}

void UAnimNotifyState_BSRotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UBSRotationComponent* RotationComp = MeshComp->GetOwner()->GetComponentByClass<UBSRotationComponent>())
	{
		RotationComp->ToggleRotation(false);
	}
}
