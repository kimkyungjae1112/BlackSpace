#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/Dialogue/BSDialogueOptionDataTable.h"
#include "BSDialogueDataTable.generated.h"

USTRUCT(BlueprintType)
struct FBSDialogueDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName RowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSoftObjectPtr<UDataTable> DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsOpenOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FBSDialogueOptionDataTable> OptionDialogues;
};