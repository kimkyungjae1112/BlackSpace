// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSBossEquipWeapon.h"

#include "Interface/BSBossWeaponEquipInterface.h"

void UAnimNotify_BSBossEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (IBSBossWeaponEquipInterface* BWE = Cast<IBSBossWeaponEquipInterface>(OwnerCharacter))
		{
			BWE->EquipWeapon();
		}
	}
}
