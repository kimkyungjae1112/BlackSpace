// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSGetUp.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "Components/BSStateComponent.h"
#include "BSGameplayTag.h"

void UAnimNotify_BSGetUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
		{
			UBSStateComponent* StateComp = OwnerCharacter->GetComponentByClass<UBSStateComponent>();
			check(StateComp);

			if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Death) == false)
			{
				if (UAnimInstance* Anim = OwnerCharacter->GetMesh()->GetAnimInstance())
				{
					Anim->Montage_Play(GetUpMontage);
				}
			}
		}
	}
}
