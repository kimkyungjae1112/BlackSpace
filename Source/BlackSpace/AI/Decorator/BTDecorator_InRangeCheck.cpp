// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_InRangeCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_InRangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return false;
	
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetBlackboardKey.SelectedKeyName));
	if (Target == nullptr) return false;

	const float Dist = AIPawn->GetDistanceTo(Target);

	return Dist >= RangeMin && Dist <= RangeMax;
}
