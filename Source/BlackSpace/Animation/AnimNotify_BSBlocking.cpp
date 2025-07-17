// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSBlocking.h"
#include "Characters/BSCharacterPlayer.h"

void UAnimNotify_BSBlocking::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (ABSCharacterPlayer* Player = Cast<ABSCharacterPlayer>(OwnerCharacter))
		{
			if (Player->OnAttackStart.IsBound())
			{
				Player->OnAttackStart.Broadcast();
			}
		}
	}
}
