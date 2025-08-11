// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDamageNumberEntryWidget.generated.h"

class UTextBlock;

UCLASS()
class BLACKSPACE_API UBSDamageNumberEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    TObjectPtr<UTextBlock> DamageText;

protected:
    float CurrentAlpha = 1.f;

public:
    void SetText(const FText& InText);
    void SetAlpha(float InAlpha);

protected:
    virtual void NativeConstruct() override;
};
