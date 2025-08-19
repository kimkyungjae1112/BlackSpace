// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSBossStageActor.generated.h"

UCLASS()
class BLACKSPACE_API ABSBossStageActor : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	double DescentDistance = 5000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	double MovementDuration = 5.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.0"))
	double EaseInExponent = 2.0;

private:
	FVector StartLocation;
	FVector TargetLocation;

	double ElapsedTime = 0.0;
	bool bIsMoving = false;

public:	
	ABSBossStageActor();

	FORCEINLINE void SetMovingFlag(const bool bInIsMoving) { bIsMoving = bInIsMoving; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
