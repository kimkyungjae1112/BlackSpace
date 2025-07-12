// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehaviorMaldrith.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BSCharacterEnemyMaldrith.h"
#include "Components/BSAttributeComponent.h"

void UBTService_SelectBehaviorMaldrith::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (IsValid(Target))
	{
		if (UBSAttributeComponent* AttributeComp = ControlledEnemy->GetComponentByClass<UBSAttributeComponent>())
		{
			if (AttributeComp->GetBaseStamina() <= StaminaCheckValue)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Strafe);
			}
			else
			{
				const float Distance = Target->GetDistanceTo(ControlledEnemy);

				if (Distance < AttackRangeDistance)
				{
					SetBehaviorKey(BlackboardComp, EAIBehavior::MeleeAttack);
				}
				else
				{
					SetBehaviorKey(BlackboardComp, EAIBehavior::Approach);
				}
			}
		}
	}
	else
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Idle);
	}
}
