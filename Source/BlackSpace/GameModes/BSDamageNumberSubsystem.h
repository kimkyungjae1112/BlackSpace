// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BSDamageNumberSubsystem.generated.h"

class UBSDamageNumberRootWidget;

USTRUCT()
struct FDamageItem
{
    GENERATED_BODY()

    /** 마지막으로 계산된 월드 위치(투영용). Target이 유효하면 매 프레임 갱신됨 */
    FVector WorldLoc = FVector::ZeroVector;

    /** 타겟 추적(액터/메시/소켓) */
    TWeakObjectPtr<AActor> TargetActor;
    TWeakObjectPtr<USkeletalMeshComponent> TargetMesh;
    FName TargetSocket = NAME_None;
    FVector LocalOffset = FVector::ZeroVector;

    /** 연출 파라미터 */
    float Damage = 0.f;
    float Life = 1.0f;
    float Elapsed = 0.f;

    /** 풀에서 빌린 인덱스 */
    int32 PoolIndex = INDEX_NONE;
};

UCLASS()
class BLACKSPACE_API UBSDamageNumberSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
    /** 뷰포트에 1장 올라가는 루트 위젯 */
    UPROPERTY()
    TObjectPtr<UBSDamageNumberRootWidget> RootWidget;

    UPROPERTY(EditDefaultsOnly, Category = "DamageNumber | UI")
    TSubclassOf<UBSDamageNumberRootWidget> RootWidgetClass;

    /** 활성 아이템 배열 */
    UPROPERTY()
    TArray<FDamageItem> Active;

protected:
    /** 오클루전(가려짐) 체크 사용 여부 */
    UPROPERTY(EditAnywhere, Category = "DamageNumber | Visibility")
    bool bUseOcclusionCheck = false;

public:
    UBSDamageNumberSubsystem();

    // UTickableWorldSubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UDamageNumberSubsystem, STATGROUP_Tickables); }

    /** 고정 좌표에 데미지 숫자 추가 */
    void AddDamage(const FVector& InWorldLoc, float InDamage, float LifeTime = 1.0f);

    /** 액터 머리 위(로컬 오프셋 포함)에 데미지 숫자 추가 */
    void AddDamageAtActor(AActor* Target, float InDamage, FVector LocalOffset = FVector(0, 0, 90.f), float LifeTime = 1.0f);

    /** 스켈레탈 메시 소켓 위치에 데미지 숫자 추가 */
    void AddDamageAtSocket(USkeletalMeshComponent* Mesh, FName Socket, float InDamage, FVector LocalOffset = FVector(0, 0, 20.f), float LifeTime = 1.0f);

private:
    void EnsureRootWidget();
    void UpdateItem(int32 Index, float DeltaTime);

};
