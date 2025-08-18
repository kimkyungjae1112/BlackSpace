// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_BSIFrame.h"

#include "Interface/BSCombatInterface.h"

void UAnimNotifyState_BSIFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (IBSCombatInterface* CombatInterface = Cast<IBSCombatInterface>(OwnerCharacter))
		{
			CombatInterface->ToggleIFrame(true);
		}
	}
}

void UAnimNotifyState_BSIFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (IBSCombatInterface* CombatInterface = Cast<IBSCombatInterface>(OwnerCharacter))
		{
			CombatInterface->ToggleIFrame(false);
		}
	}
}
