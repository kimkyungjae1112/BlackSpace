// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSWeaponEquip.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "Components/BSCombatComponent.h"
#include "Equipments/BSWeapon.h"
#include "BSGameplayTag.h"

void UAnimNotify_BSWeaponEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacter* Player = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UAnimInstance* Anim = Player->GetMesh()->GetAnimInstance())
		{
			if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
			{
				if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
				{
					UAnimMontage* CurrentMontage = Anim->GetCurrentActiveMontage();
					float CurrentPosition = Anim->Montage_GetPosition(CurrentMontage);

					CombatComp->ChangeWeapon();

					Weapon = CombatComp->GetMainWeapon();

					UAnimMontage* NewMontage = Weapon->GetMontageForTag(BSGameplayTag::Character_Action_Equip);
					Anim->Montage_Play(NewMontage);
					Anim->Montage_SetPosition(NewMontage, CurrentPosition);
				}
			}
		}
	}
}