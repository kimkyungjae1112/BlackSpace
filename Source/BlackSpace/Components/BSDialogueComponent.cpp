// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSDialogueComponent.h"
#include "Components/ScrollBox.h"

#include "UI/Dialogue/BSDialogueWidget.h"
#include "UI/Dialogue/BSDialogueOptionWidget.h"
#include "Interface/BSDialogueEndInterface.h"
#include "Player/BSPlayerController.h"

UBSDialogueComponent::UBSDialogueComponent()
{

}

void UBSDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	DialogueTable = *NormalDialogueTable->FindRow<FBSDialogueDataTable>(TEXT("1"), TEXT(""));
}

void UBSDialogueComponent::StartDialogue()
{
	DialogueWidget = CreateWidget<UBSDialogueWidget>(GetWorld(), DialogueWidgetClass);
	DialogueWidget->AddToViewport();
	DialogueWidget->SetDialogueText(NPCName, DialogueTable.Dialogue);
}

void UBSDialogueComponent::NextDialogue()
{
	if (DialogueTable.bIsEnd)
	{
		DialogueWidget->RemoveFromParent();

		if (APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (IBSDialogueEndInterface* DialogueEndInterface = Cast<IBSDialogueEndInterface>(Player))
			{
				DialogueEndInterface->EndDialogue();
			}
		}

		DialogueTable = *DialogueTable.DataTable->FindRow<FBSDialogueDataTable>(TEXT("1"), TEXT(""));

		return;
	}
	else if (DialogueTable.bIsOpenOption)
	{
		if (ABSPlayerController* PC = Cast<ABSPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PC->SetInputModeUIAndGame();
		}

		for (const auto& OptionDataTable : DialogueTable.OptionDialogues)
		{
			DialogueOptionSpawn(OptionDataTable);
		}

		return;
	}

	if (!DialogueTable.DataTable.IsValid())
	{
		DialogueTable.DataTable = DialogueTable.DataTable.LoadSynchronous();
	}

	DialogueTable = *DialogueTable.DataTable->FindRow<FBSDialogueDataTable>(DialogueTable.RowIndex, TEXT(""));

	DialogueWidget->SetDialogueText(NPCName, DialogueTable.Dialogue);
}

void UBSDialogueComponent::SkipDialogue()
{
	DialogueWidget->RemoveFromParent();

	if (APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (IBSDialogueEndInterface* DialogueEndInterface = Cast<IBSDialogueEndInterface>(Player))
		{
			DialogueEndInterface->EndDialogue();
		}
	}

	DialogueTable = *DialogueTable.DataTable->FindRow<FBSDialogueDataTable>(TEXT("1"), TEXT(""));

}

void UBSDialogueComponent::OnClickButton(const FName& InIndex)
{
	DialogueWidget->GetOptionScrollBox()->ClearChildren();

	DialogueTable = *DialogueTable.DataTable->FindRow<FBSDialogueDataTable>(InIndex, TEXT(""));
	DialogueWidget->SetDialogueText(NPCName, DialogueTable.Dialogue);
}

void UBSDialogueComponent::DialogueOptionSpawn(const FBSDialogueOptionDataTable& OptionDataTable)
{
	UBSDialogueOptionWidget* DialogueOptionWidget = CreateWidget<UBSDialogueOptionWidget>(GetWorld(), DialogueOptionWidgetClass);
	DialogueOptionWidget->SetDialogueComp(this, OptionDataTable.RowIndex);
	DialogueOptionWidget->SetDialogueText(OptionDataTable.OptionDialogue);

	DialogueWidget->GetOptionScrollBox()->AddChild(DialogueOptionWidget);
}



