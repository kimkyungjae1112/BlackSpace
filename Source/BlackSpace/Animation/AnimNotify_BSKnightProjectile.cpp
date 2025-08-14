// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSKnightProjectile.h"

#include "Projectiles/BSKnightRangedAttackProjectile.h"

void UAnimNotify_BSKnightProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (AActor* CharacterOwner = MeshComp->GetOwner())
	{
		if (UWorld* World = CharacterOwner->GetWorld())
		{
			FActorSpawnParameters Param;
			Param.Owner = CharacterOwner;

			if (SpawnSocketName.IsValid())
			{
				UE_LOG(LogTemp, Display, TEXT("Spawn"));
				FTransform Transform = MeshComp->GetSocketTransform(SpawnSocketName);

				ABSKnightRangedAttackProjectile* Projectile = World->SpawnActor<ABSKnightRangedAttackProjectile>(ProjectileClass, Transform, Param);
				Projectile->SetDirection(CharacterOwner->GetActorForwardVector());
			}
		}
	}
}
