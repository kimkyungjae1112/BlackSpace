// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDefine.h"
#include "BSBossHealthBarWidget.generated.h"

class UBSAttributeComponent;
class UBSStatBarWidget;
class UTextBlock;

UCLASS()
class BLACKSPACE_API UBSBossHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> NameWidget;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSStatBarWidget> HealthBarWidget;

public:
	void Init(UBSAttributeComponent* AttributeComp, FText ThisName);

protected:
	void OnAttributeChanged(const EAttributeType& AttributeType, float Ratio);
};
