// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSBowFireFinished.generated.h"

UCLASS(meta=(DisplayName = "Bow Fire Finished"))
class BLACKSPACE_API UAnimNotify_BSBowFireFinished : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_BSBowFireFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
