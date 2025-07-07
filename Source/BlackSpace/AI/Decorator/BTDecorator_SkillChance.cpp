// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_SkillChance.h"

bool UBTDecorator_SkillChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return ChanceRate >= FMath::RandRange(1, 100);
}
