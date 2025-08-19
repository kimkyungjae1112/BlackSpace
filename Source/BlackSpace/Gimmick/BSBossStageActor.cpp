// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSBossStageActor.h"

ABSBossStageActor::ABSBossStageActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABSBossStageActor::BeginPlay()
{
	Super::BeginPlay();
	
	Super::BeginPlay();

	// 이동 시작 시, 초기 위치와 목표 위치를 설정합니다.
	StartLocation = GetActorLocation();
	TargetLocation = StartLocation - FVector(0.0, 0.0, DescentDistance);

	ElapsedTime = 0.0;
}

void ABSBossStageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		// 경과 시간을 누적합니다.
		ElapsedTime += DeltaTime;

		// 전체 시간에 대한 현재 진행률(0.0 ~ 1.0)을 계산합니다.
		const double RawAlpha = FMath::Clamp(ElapsedTime / MovementDuration, 0.0, 1.0);

		// 진행률에 제곱을 하여 Ease-In(처음엔 느리게) 효과를 만듭니다.
		const double EasedAlpha = FMath::Pow(RawAlpha, EaseInExponent);

		// Lerp (선형 보간)를 사용해 시작 위치와 목표 위치 사이의 현재 위치를 계산합니다.
		const FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, EasedAlpha);
		SetActorLocation(NewLocation);

		// 이동이 완료되었는지 확인합니다.
		if (ElapsedTime >= MovementDuration)
		{
			bIsMoving = false; // 이동 중지
			Destroy();         // 액터 파괴
		}
	}
}

