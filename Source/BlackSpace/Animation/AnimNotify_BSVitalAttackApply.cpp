// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSVitalAttackApply.h"

#include "Interface/BSEnemyInterface.h"
#include "Characters/BSCharacterPlayer.h"

void UAnimNotify_BSVitalAttackApply::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (ABSCharacterPlayer* Player = Cast<ABSCharacterPlayer>(OwnerCharacter))
		{
			if (IBSEnemyInterface* Interface = Cast<IBSEnemyInterface>(Player->GetVitalAttackTarget()))
			{
				Interface->ApplyVitalAttack(VitalAttackDamage);
			}
		}
	}
}
