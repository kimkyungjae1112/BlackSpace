// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_BSComboWindow.h"

#include "Interface/BSComboWindowInterface.h"


UAnimNotifyState_BSComboWindow::UAnimNotifyState_BSComboWindow(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotifyState_BSComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IBSComboWindowInterface* Interface = Cast<IBSComboWindowInterface>(MeshComp->GetOwner()))
	{
		Interface->EnableComboWindow();
	}
}

void UAnimNotifyState_BSComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IBSComboWindowInterface* Interface = Cast<IBSComboWindowInterface>(MeshComp->GetOwner()))
	{
		Interface->DisableComboWindow();
	}
}
