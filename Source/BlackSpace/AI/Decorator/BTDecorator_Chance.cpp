// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_Chance.h"

bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return ChanceRate >= FMath::RandRange(1, 100);
}
