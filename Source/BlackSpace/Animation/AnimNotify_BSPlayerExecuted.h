// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AnimNotify_BSPlayerExecuted.generated.h"

class UNiagaraSystem;

UCLASS(meta=(DisplayName="Player Executed"))
class BLACKSPACE_API UAnimNotify_BSPlayerExecuted : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ExecutedMontage;

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	float Radius = 100.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> Template;

public:
	UAnimNotify_BSPlayerExecuted(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
 