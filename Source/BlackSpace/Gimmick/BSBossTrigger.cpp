// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSBossTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "GameFramework/PlayerController.h"

#include "Interface/BSBossWeaponEquipInterface.h"
#include "Characters/BSCharacterEnemyKnight.h"
#include "Characters/BSCharacterEnemyMaldrith.h"
#include "Gimmick/BSBossStageActor.h"

ABSBossTrigger::ABSBossTrigger()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);

	static ConstructorHelpers::FObjectFinder<ULevelSequence> LevelSequenceAssetRef(TEXT("/Script/LevelSequence.LevelSequence'/Game/_Game/Blueprints/LevelSequence/NewLevelSequence.NewLevelSequence'"));
	if (LevelSequenceAssetRef.Object)
	{
		LevelSequenceAsset = LevelSequenceAssetRef.Object;
	}
}

void ABSBossTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSBossTrigger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		ULevelSequence* Sequence = LevelSequenceAsset.Get();
		ALevelSequenceActor* SequenceActor;
		if (Sequence)
		{
			// 시퀀스 플레이어와 액터 생성
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(),
				Sequence,
				PlaybackSettings,
				SequenceActor
			);
		}

		if (LevelSequencePlayer)
		{
			LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnSequenceFinished);
			LevelSequencePlayer->Play();

			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				if (KnightAppearCameraShake)
				{
					PC->ClientStartCameraShake(KnightAppearCameraShake);
				}
			}
		}

		if (ABSBossStageActor* BossStageActor = Cast<ABSBossStageActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABSBossStageActor::StaticClass())))
		{
			BossStageActor->SetMovingFlag(true);
		}

		switch (BossType)
		{
		case EBossType::Maldrith:
			if (IBSBossWeaponEquipInterface* BWE = Cast<IBSBossWeaponEquipInterface>(UGameplayStatics::GetActorOfClass(GetWorld(), ABSCharacterEnemyMaldrith::StaticClass())))
			{
				BWE->PlayEquipMontage();
			}
			break;
		case EBossType::Knight:
			if (IBSBossWeaponEquipInterface* BWE = Cast<IBSBossWeaponEquipInterface>(UGameplayStatics::GetActorOfClass(GetWorld(), ABSCharacterEnemyKnight::StaticClass())))
			{
				BWE->PlayEquipMontage();
			}
			break;
		}
	}
}

void ABSBossTrigger::OnSequenceFinished()
{


	Destroy();
}
