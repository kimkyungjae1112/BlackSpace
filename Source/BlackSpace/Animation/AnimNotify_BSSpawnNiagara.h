// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BSSpawnNiagara.generated.h"

class UNiagaraSystem;

UCLASS(meta=(DisplayName="Spawn Niagara"))
class BLACKSPACE_API UAnimNotify_BSSpawnNiagara : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> Template;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FVector Scale = FVector(1.0f);

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
