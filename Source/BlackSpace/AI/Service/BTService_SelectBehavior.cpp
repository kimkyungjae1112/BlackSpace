// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehavior.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/BSCharacterEnemy.h"
#include "Components/BSStateComponent.h"

UBTService_SelectBehavior::UBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

}

void UBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (ABSCharacterEnemy* ControlledEnemy = Cast<ABSCharacterEnemy>(ControlledPawn))
		{
			UpdateBehavior(OwnerComp.GetBlackboardComponent(), ControlledEnemy);
		}
	}
}

void UBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* BlackboardComp, const EAIBehavior& Behavior) const
{
	BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(Behavior));
}

void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	UBSStateComponent* StateComp = ControlledEnemy->GetComponentByClass<UBSStateComponent>();
	check(StateComp);

	if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Parried))
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Stunned);
	}
	else
	{
		if (IsValid(TargetActor))
		{
			const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

			if (Distance <= AttackRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::MeleeAttack);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Approach);
			}
		}
		else
		{
			if (ControlledEnemy->GetPatrolPoint() != nullptr)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Idle);
			}
		}
	}
}
