// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

#include "GameModes/BSAudioManagerSubsystem.h"
#include "Gimmick/BSCheckPoint.h"
#include "Characters/BSCharacterPlayer.h"
#include "Player/BSPlayerController.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSStateComponent.h"

void ABSGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}

	if (UBSAudioManagerSubsystem* AudioManager = GetWorld()->GetGameInstance()->GetSubsystem<UBSAudioManagerSubsystem>())
	{
		AudioManager->PlayMusic(BGM, true);
	}
}

void ABSGameMode::SetCheckpoint(ABSCheckPoint* InCheckPoint)
{
	CheckPoint = InCheckPoint;
}

void ABSGameMode::RespawnPlayer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || !CheckPoint) 
	{ 
		return; 
	}

	ACharacter* Player = Cast<ACharacter>(PC->GetPawn());
	if (!Player) 
	{ 
		return; 
	}

	const FTransform SpawnTM = CheckPoint->GetPosComp()->GetComponentToWorld();

	Player->TeleportTo(SpawnTM.GetLocation(), SpawnTM.Rotator(), false, true);

	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player->EnableInput(PC);

	Player->FindComponentByClass<UBSAttributeComponent>()->ResetAttribute();
	Player->FindComponentByClass<UBSStateComponent>()->ToggleMovementInput(true);
	Player->FindComponentByClass<UBSStateComponent>()->RespawnState();

	Player->GetMesh()->SetSimulatePhysics(false);
	Player->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	FVector MeshLocation = Player->GetMesh()->GetComponentLocation();
	FVector CapsuleLocation = Player->GetCapsuleComponent()->GetComponentLocation();
	FVector Delta = MeshLocation - CapsuleLocation;
	Delta.Z = 0; 
	Player->GetCapsuleComponent()->SetWorldLocation(MeshLocation);

	Player->GetMesh()->AttachToComponent(Player->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Player->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -(Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight())));
	Player->GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}
