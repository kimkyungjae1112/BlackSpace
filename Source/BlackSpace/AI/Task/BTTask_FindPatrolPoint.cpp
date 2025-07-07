// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_FindPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BSCharacterEnemy.h"
#include "Engine/TargetPoint.h"

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return EBTNodeResult::Failed;

	if (ABSCharacterEnemy* EnemyPawn = Cast<ABSCharacterEnemy>(AIPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName, EnemyPawn->GetPatrolPoint()->GetActorLocation());
		EnemyPawn->IncrementPatrolIndex();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
