// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSBackAttackEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


#include "Components/BSCombatComponent.h"
#include "Equipments/BSWeapon.h"

void UAnimNotify_BSBackAttackEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (UBSCombatComponent* CombatComp = OwnerCharacter->GetComponentByClass<UBSCombatComponent>())
		{
			if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				const FVector EffectLoc = Weapon->MeshComp->GetSocketLocation(TEXT("BackAttackEffect"));
				const FRotator EffectRoc = Weapon->MeshComp->GetSocketRotation(TEXT("BackAttackEffect"));

				if (HitEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						OwnerCharacter,
						HitEffect,
						EffectLoc,
						EffectRoc
					);
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySound2D(OwnerCharacter, HitSound);
				}
			}
		}
	}
}
