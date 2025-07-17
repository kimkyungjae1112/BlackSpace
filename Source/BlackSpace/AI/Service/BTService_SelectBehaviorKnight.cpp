// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehaviorKnight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BSCharacterEnemyKnight.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSStateComponent.h"
#include "BSGameplayTag.h"

void UBTService_SelectBehaviorKnight::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	UBSStateComponent* StateComp = ControlledEnemy->GetComponentByClass<UBSStateComponent>();
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Parried);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);

	if (StateComp->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Stunned);
	}
	else
	{
		if (IsValid(Target) && StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Blocking))
		{
			SetBehaviorKey(BlackboardComp, EAIBehavior::Blocking);
		}
		else if (IsValid(Target))
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
}
