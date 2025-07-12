// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterEnemy.h"
#include "BSCharacterEnemyMaldrith.generated.h"

class UBSBossHealthBarWidget;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemyMaldrith : public ABSCharacterEnemy
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Name")
	FText BossName;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | UI")
	TSubclassOf<UBSBossHealthBarWidget> BossHealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | UI")
	TObjectPtr<UBSBossHealthBarWidget> BossHealthBarWidget;

public:
	ABSCharacterEnemyMaldrith();

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void SeesTarget(AActor* InTargetActor) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;
};
