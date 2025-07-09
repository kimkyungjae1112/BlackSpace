// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSGameplayTag.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BSStateComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/* 이동키 입력 상태 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bMovementInputEnabled = true;

	/* 캐릭터의 현재 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTag CurrentState;

public:	
	UBSStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE bool GetMovementInputEnabled() const { return bMovementInputEnabled; }
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; }
	FORCEINLINE void SetState(const FGameplayTag& NewState) 
	{
		if (CurrentState == BSGameplayTag::Character_State_Death) return;

		CurrentState = NewState; 
	}

public:
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);

	/* 상태 초기화 */
	void ClearState();

	/* 현재 상태가 TagsToCheck 중에 있나? */
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;
	bool IsCurrentStateEqualToIt(const FGameplayTag& TagToCheck) const;
		
protected:
	UFUNCTION()
	void MovementInputEnableAction();

};
