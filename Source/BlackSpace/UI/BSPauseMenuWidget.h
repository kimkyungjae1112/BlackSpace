// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSPauseMenuWidget.generated.h"

class UButton;

UCLASS()
class BLACKSPACE_API UBSPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> SettingButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> QuitButton;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ResumeButtonHandle();

	UFUNCTION()
	void SettingButtonHandle();

	UFUNCTION()
	void MainMenuButtonHandle();

	UFUNCTION()
	void QuitButtonHandle();
};
