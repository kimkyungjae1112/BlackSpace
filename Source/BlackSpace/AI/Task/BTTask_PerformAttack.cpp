// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_PerformAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BSCombatInterface.h"
#include "Components/BSStateComponent.h"

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return EBTNodeResult::Failed;

	if (IBSCombatInterface* CombatInterface = Cast<IBSCombatInterface>(AIPawn))
	{
		FOnMontageEnded MontageEnd;
		MontageEnd.BindLambda([this, &OwnerComp, AIPawn](UAnimMontage*, bool bInterrupted)
			{
				if (IsValid(&OwnerComp) == false)
				{
					return;
				}

				if (UBSStateComponent* StateComp = AIPawn->GetComponentByClass<UBSStateComponent>())
				{
					StateComp->ClearState();
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});

		CombatInterface->PerformAttack(AttackTypeTag, MontageEnd);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
