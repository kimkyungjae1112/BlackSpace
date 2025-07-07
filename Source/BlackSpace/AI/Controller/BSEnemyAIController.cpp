// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/BSEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

#include "Characters/BSCharacterEnemy.h"
#include "Components/BSRotationComponent.h"

ABSEnemyAIController::ABSEnemyAIController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
}

void ABSEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<ABSCharacterEnemy>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

	GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void ABSEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	ControlledEnemy = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(PerceptionTimerHandle);
}

void ABSEnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComp->GetKnownPerceivedActors(nullptr, OutActors);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (OutActors.Contains(PlayerCharacter))
	{
		SetTarget(PlayerCharacter);
		ControlledEnemy->ToggleHealthBarVisibility(true);
	}
	else
	{
		SetTarget(nullptr);
		ControlledEnemy->ToggleHealthBarVisibility(false);
	}

}

void ABSEnemyAIController::SetTarget(AActor* NewTarget) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(TEXT("Target"), NewTarget);
	}

	if (IsValid(ControlledEnemy))
	{
		if (UBSRotationComponent* RotationComp = ControlledEnemy->GetComponentByClass<UBSRotationComponent>())
		{
			RotationComp->SetTargetActor(NewTarget);
		}
	}
	
}
