// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSStatBarWidget.generated.h"

class UProgressBar;

UCLASS()
class BLACKSPACE_API UBSStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

public:
	UBSStatBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void SetStatBarRatio(float InRatio);
};
