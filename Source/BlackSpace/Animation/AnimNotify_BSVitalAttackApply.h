// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSVitalAttackApply.generated.h"

UCLASS(meta=(DisplayName="Vital Attack Apply"))
class BLACKSPACE_API UAnimNotify_BSVitalAttackApply : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float VitalAttackDamage = 150.f;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
