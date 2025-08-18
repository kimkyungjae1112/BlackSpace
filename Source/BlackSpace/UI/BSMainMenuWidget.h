// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSMainMenuWidget.generated.h"

class UButton;

UCLASS()
class BLACKSPACE_API UBSMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> TutorialButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> SettingButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> ExitButton;

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void TutorialStart();

	UFUNCTION()
	void SettingStart();

	UFUNCTION()
	void ExitStart();
};
