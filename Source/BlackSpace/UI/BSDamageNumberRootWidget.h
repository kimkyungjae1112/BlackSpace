// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDamageNumberRootWidget.generated.h"

class UCanvasPanel;
class UBSDamageNumberEntryWidget;

USTRUCT()
struct FEntrySlot
{
    GENERATED_BODY()

    UPROPERTY() 
    TObjectPtr<UBSDamageNumberEntryWidget> Widget;

    bool InUse = false;
};

UCLASS()
class BLACKSPACE_API UBSDamageNumberRootWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    TObjectPtr<UCanvasPanel> Canvas;

    UPROPERTY(EditAnywhere, Category = "DamageNumber | UI")
    TSubclassOf<UBSDamageNumberEntryWidget> EntryClass;

    UPROPERTY()
    TArray<FEntrySlot> Pool;

public:
    /** 초기 풀 생성 */
    void InitPool(int32 InitialSize);

    /** 새로운 데미지 요청 시 하나 빌림(텍스트 초기화) */
    int32 BorrowEntry(const FText& DamageText);

    /** 반환(숨김/자리 비움) */
    void ReturnEntry(int32 Index);

    /** 매 프레임 갱신(서브시스템에서 호출) */
    void UpdateEntry(int32 Index, const FVector2D& ScreenPos, float Alpha, float Scale, bool bVisible);

protected:
    virtual void NativeConstruct() override;

private:
    int32 AddOne();
    int32 FindFreeIndex() const;
};
