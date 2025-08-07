// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDialogueWidget.generated.h"

class UTextBlock;
class UScrollBox;

UCLASS()
class BLACKSPACE_API UBSDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> SpeakerTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> DialogueTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UScrollBox> OptionScrollBox;

public:
	UBSDialogueWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UScrollBox* GetOptionScrollBox() const;

protected:
	virtual void NativeConstruct() override;

public:
	void SetDialogueText(const FText& InSpeaker, const FText& InDialogue) const;
};
