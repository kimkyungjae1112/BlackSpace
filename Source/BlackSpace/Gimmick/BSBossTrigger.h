// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSBossTrigger.generated.h"

class UBoxComponent;
class ULevelSequence;
class ULevelSequencePlayer;
class UCameraShakeBase;

UENUM(BlueprintType)
enum class EBossType : uint8
{
	Maldrith,
	Knight,
};

UCLASS()
class BLACKSPACE_API ABSBossTrigger : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Sequence")
	TSoftObjectPtr<ULevelSequence> LevelSequenceAsset;

	UPROPERTY(VisibleAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TSubclassOf<UCameraShakeBase> KnightAppearCameraShake;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditAnywhere, Category = "Boss Type")
	EBossType BossType;

public:	
	ABSBossTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSequenceFinished();
};
