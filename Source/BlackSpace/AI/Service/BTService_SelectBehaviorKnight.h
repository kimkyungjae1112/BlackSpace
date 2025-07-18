// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Service/BTService_SelectBehavior.h"
#include "BTService_SelectBehaviorKnight.generated.h"

UCLASS()
class BLACKSPACE_API UBTService_SelectBehaviorKnight : public UBTService_SelectBehavior
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float StaminaCheckValue = 40.f;

protected:
	virtual void UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const override;
};
