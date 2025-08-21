// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSKnockBack.generated.h"

UCLASS(meta=(DisplayName="KnockBack Attack"))
class BLACKSPACE_API UAnimNotify_BSKnockBack : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<AActor> AlreadyHitActor;

protected:
	UPROPERTY(EditAnywhere)
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere)
	FName LocationSocketName;

	UPROPERTY(EditAnywhere)
	float DamageRadius = 50.f;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
