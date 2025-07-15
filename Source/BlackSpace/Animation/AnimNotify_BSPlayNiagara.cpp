// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSPlayNiagara.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Components/BSCombatComponent.h"
#include "Equipments/BSWeapon.h"

void UAnimNotify_BSPlayNiagara::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (UBSCombatComponent* CombatComp = OwnerCharacter->GetComponentByClass<UBSCombatComponent>())
		{
			if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				const FVector EffectLoc = Weapon->MeshComp->GetSocketLocation(SocketName);
				const FRotator EffectRoc = Weapon->MeshComp->GetSocketRotation(SocketName);

				if (HitEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						OwnerCharacter,
						HitEffect,
						EffectLoc,
						EffectRoc
					);
				}
			}
		}
	}
}
