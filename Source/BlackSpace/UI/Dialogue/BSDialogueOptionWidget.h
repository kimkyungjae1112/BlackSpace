// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDialogueOptionWidget.generated.h"

class UTextBlock;
class UButton;
class UBSDialogueComponent;

UCLASS()
class BLACKSPACE_API UBSDialogueOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> Dialogue;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBSDialogueComponent> DialogueComp;

	UPROPERTY(VisibleAnywhere)
	FName Index;

public:
	UBSDialogueOptionWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetDialogueComp(UBSDialogueComponent* InDialogueComp, const FName& InIndex);
	void SetDialogueText(const FText& InDialogue);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickButton();
};
