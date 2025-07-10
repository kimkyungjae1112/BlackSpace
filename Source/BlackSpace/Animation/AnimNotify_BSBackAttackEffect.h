// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSBackAttackEffect.generated.h"

class UNiagaraSystem;
class USoundCue;

UCLASS(meta=(DisplayName = "Back Attack Effect"))
class BLACKSPACE_API UAnimNotify_BSBackAttackEffect : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> HitSound;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
