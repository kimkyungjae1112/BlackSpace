// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSKnightProjectile.generated.h"

class ABSKnightRangedAttackProjectile;

UCLASS(meta = (DisplayName = "Kight Projectile Fire"))
class BLACKSPACE_API UAnimNotify_BSKnightProjectile : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABSKnightRangedAttackProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	FName SpawnSocketName;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
