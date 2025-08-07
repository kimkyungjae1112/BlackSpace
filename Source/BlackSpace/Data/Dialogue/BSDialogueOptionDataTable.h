#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BSDialogueOptionDataTable.generated.h"

USTRUCT(BlueprintType)
struct FBSDialogueOptionDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName RowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OptionDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSoftObjectPtr<UDataTable> DataTable;
};