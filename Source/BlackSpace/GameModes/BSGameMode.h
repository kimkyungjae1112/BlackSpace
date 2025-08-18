// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BSGameMode.generated.h"

class ABSCheckPoint;

UCLASS()
class BLACKSPACE_API ABSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Set Value")
	bool bIsTutorial = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> BGM;

	UPROPERTY(EditAnywhere, Category = "CheckPoint")
	TObjectPtr<ABSCheckPoint> CheckPoint;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsTutorial() const { return bIsTutorial; }
	void SetCheckpoint(ABSCheckPoint* InCheckPoint);
	void RespawnPlayer();
};

