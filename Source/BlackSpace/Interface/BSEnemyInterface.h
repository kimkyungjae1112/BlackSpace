// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSEnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSEnemyInterface
{
	GENERATED_BODY()

public:
	virtual void ToggleBackAttackWidgetVisibility(const bool bShouldBackAttack) = 0;
	virtual void BackAttacked(UAnimMontage* BackAttackReactionMontage) = 0;
};
