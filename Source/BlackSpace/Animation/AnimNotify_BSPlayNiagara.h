// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSPlayNiagara.generated.h"

class UNiagaraSystem;

UCLASS(meta = (DisplayName="BS Play Niagara"))
class BLACKSPACE_API UAnimNotify_BSPlayNiagara : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere)
	FName SocketName;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
