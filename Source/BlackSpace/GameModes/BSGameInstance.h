// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BSGameInstance.generated.h"

UCLASS()
class BLACKSPACE_API UBSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Loading")
	TSoftClassPtr<UUserWidget> LoadingScreenClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveLoadingWidget;

public:
	FORCEINLINE UUserWidget* GetLoadingScreen() const { return nullptr; }

	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void ShowLoadingScreen(float MinDisplayTime = 0.5f);

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void HideLoadingScreen();

protected:
	void HandlePreLoadMap(const FString& MapName);
	void HandlePostLoadMap(UWorld* LoadedWorld);
};
