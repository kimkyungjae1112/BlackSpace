// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_UpdateMoveSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_UpdateMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

    ACharacter* AIPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AIPawn == nullptr) return EBTNodeResult::Failed;

    UCharacterMovementComponent* Movement = AIPawn->GetCharacterMovement();
    if (Movement == nullptr) return EBTNodeResult::Failed;

    Movement->MaxWalkSpeed = MaxWalkSpeed;

    return EBTNodeResult::Succeeded;
}
