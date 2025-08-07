// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialogue/BSDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

UBSDialogueWidget::UBSDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UScrollBox* UBSDialogueWidget::GetOptionScrollBox() const
{
	return OptionScrollBox;
}

void UBSDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBSDialogueWidget::SetDialogueText(const FText& InSpeaker, const FText& InDialogue) const
{
	SpeakerTextBlock->SetText(InSpeaker);
	DialogueTextBlock->SetText(InDialogue);
}
