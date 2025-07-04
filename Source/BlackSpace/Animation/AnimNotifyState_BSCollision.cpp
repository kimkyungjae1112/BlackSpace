// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_BSCollision.h"

#include "Components/BSWeaponCollisionComponent.h"
#include "Components/BSCombatComponent.h"
#include "Equipments/BSWeapon.h"

UAnimNotifyState_BSCollision::UAnimNotifyState_BSCollision(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotifyState_BSCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (UBSCombatComponent* CombatComp = OwnerCharacter->GetComponentByClass<UBSCombatComponent>())
		{
			if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				if (UBSWeaponCollisionComponent* CollisionComp = Weapon->GetCollision())
				{
					CollisionComp->TurnOnCollision();
				}
			}
		}
	}
}

void UAnimNotifyState_BSCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (UBSCombatComponent* CombatComp = OwnerCharacter->GetComponentByClass<UBSCombatComponent>())
		{
			if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				if (UBSWeaponCollisionComponent* CollisionComp = Weapon->GetCollision())
				{
					CollisionComp->TurnOffCollision();
				}
			}
		}
	}
}
