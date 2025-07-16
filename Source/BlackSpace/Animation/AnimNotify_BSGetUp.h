// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSGetUp.generated.h"

UCLASS(meta = (DisplayName="Get Up"))
class BLACKSPACE_API UAnimNotify_BSGetUp : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> GetUpMontage;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
