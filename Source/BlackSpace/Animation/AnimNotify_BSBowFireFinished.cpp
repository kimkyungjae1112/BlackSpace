// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSBowFireFinished.h"
#include "Interface/BSBowFireInterface.h"

UAnimNotify_BSBowFireFinished::UAnimNotify_BSBowFireFinished(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotify_BSBowFireFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IBSBowFireInterface* Interface = Cast<IBSBowFireInterface>(MeshComp->GetOwner()))
	{
		Interface->BowFireFinished();
	}
}
