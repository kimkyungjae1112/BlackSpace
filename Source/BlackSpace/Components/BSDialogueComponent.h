// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Dialogue/BSDialogueDataTable.h"
#include "Data/Dialogue/BSDialogueOptionDataTable.h"
#include "BSDialogueComponent.generated.h"

class UBSDialogueWidget;
class UBSDialogueOptionWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

// Name
protected:
	UPROPERTY(EditAnywhere, Category = "Dialogue | Name")
	FText NPCName;

// UI
protected:
	UPROPERTY(EditAnywhere, Category = "Dialogue | UI")
	TSubclassOf<UBSDialogueWidget> DialogueWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Dialogue | UI")
	TObjectPtr<UBSDialogueWidget> DialogueWidget;

	UPROPERTY(EditAnywhere, Category = "Dialogue | UI")
	TSubclassOf<UBSDialogueOptionWidget> DialogueOptionWidgetClass;

// Dialogue
protected:
	UPROPERTY(EditAnywhere, Category = "Dialogue | Data")
	TObjectPtr<UDataTable> NormalDialogueTable;

	UPROPERTY(VisibleAnywhere, Category = "Dialogue | Data")
	FBSDialogueDataTable DialogueTable;

	UPROPERTY(VisibleAnywhere)
	FName RowIndex;

	UPROPERTY(VisibleAnywhere)
	bool bIsEnd;

	UPROPERTY(VisibleAnywhere)
	FName PrevIndex;

public:	
	UBSDialogueComponent();

protected:
	virtual void BeginPlay() override;

public:
	void StartDialogue();
	void NextDialogue();
	void SkipDialogue();
	void OnClickButton(const FName& InIndex);

private:
	void DialogueOptionSpawn(const FBSDialogueOptionDataTable& OptionDataTable);
};
