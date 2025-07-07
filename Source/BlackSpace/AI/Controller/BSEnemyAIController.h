// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BSEnemyAIController.generated.h"

class ABSCharacterEnemy;

UCLASS()
class BLACKSPACE_API ABSEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	ABSCharacterEnemy* ControlledEnemy;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;

	FTimerHandle PerceptionTimerHandle;

public:
	ABSEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
};
