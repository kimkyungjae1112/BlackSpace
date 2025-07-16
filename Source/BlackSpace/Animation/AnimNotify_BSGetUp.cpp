// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSGetUp.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

void UAnimNotify_BSGetUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UAnimInstance* Anim = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(GetUpMontage);
		}
	}
}
