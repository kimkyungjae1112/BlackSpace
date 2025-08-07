// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialogue/BSDialogueOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Components/BSDialogueComponent.h"

UBSDialogueOptionWidget::UBSDialogueOptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSDialogueOptionWidget::SetDialogueComp(UBSDialogueComponent* InDialogueComp, const FName& InIndex)
{
	DialogueComp = InDialogueComp;
	Index = InIndex;
}

void UBSDialogueOptionWidget::SetDialogueText(const FText& InDialogue)
{
	Dialogue->SetText(InDialogue);
}

void UBSDialogueOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OptionButton->OnClicked.AddDynamic(this, &UBSDialogueOptionWidget::OnClickButton);
}

void UBSDialogueOptionWidget::OnClickButton()
{
	DialogueComp->OnClickButton(Index);
}
