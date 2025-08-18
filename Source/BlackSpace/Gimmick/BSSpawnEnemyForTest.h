// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSSpawnEnemyForTest.generated.h"

class ATargetPoint;
class ABSCharacterEnemy;
class UBoxComponent;

UCLASS()
class BLACKSPACE_API ABSSpawnEnemyForTest : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UBoxComponent> BoxComp;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ATargetPoint> SpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABSCharacterEnemy> SpawnEnemyClass;

public:	
	ABSSpawnEnemyForTest();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
