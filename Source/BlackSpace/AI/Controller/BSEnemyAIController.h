// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/BSAIControllerInterface.h"
#include "BSEnemyAIController.generated.h"

class ABSCharacterEnemy;

UCLASS()
class BLACKSPACE_API ABSEnemyAIController 
	: public AAIController
	, public IBSAIControllerInterface
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

public:
	virtual bool IsDetectedPlayer() const override;

	void StopUpdateTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
};
