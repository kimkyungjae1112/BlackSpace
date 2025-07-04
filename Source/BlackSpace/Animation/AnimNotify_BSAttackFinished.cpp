// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSAttackFinished.h"
#include "Characters/BSCharacterPlayer.h"
#include "Interface/BSComboWindowInterface.h"

UAnimNotify_BSAttackFinished::UAnimNotify_BSAttackFinished(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotify_BSAttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ABSCharacterPlayer* Player = Cast<ABSCharacterPlayer>(MeshComp->GetOwner()))
	{
		Player->AttackFinished(ComboResetDelay);
	}

}
