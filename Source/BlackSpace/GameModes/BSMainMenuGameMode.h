// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BSMainMenuGameMode.generated.h"

class UBSMainMenuWidget;

UCLASS()
class BLACKSPACE_API ABSMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UBSMainMenuWidget> MainMenuWidgetClass;

public:
	ABSMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
};
