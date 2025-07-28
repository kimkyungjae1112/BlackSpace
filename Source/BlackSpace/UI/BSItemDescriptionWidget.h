// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDefine.h"
#include "BSItemDescriptionWidget.generated.h"

class UVerticalBox;
class UTextBlock;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSItemDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UVerticalBox> EntireBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> GradeText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> TypeText;

public:
	UBSItemDescriptionWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetDescription(const FInventorySlot& InDescriptionSlot);
	void UnsetDescription();

private:
	FText GetWeaponGradeText(const EWeaponGrade& Grade);
};
