// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSGameInstance.h"
#include "GenericTeamAgentInterface.h"

#include "UI/BSInventoryMenuWidget.h"

static ETeamAttitude::Type BSGetTeamAttitude(FGenericTeamId TeamA, FGenericTeamId TeamB)
{
    if (TeamA != TeamB) 
    {
        return ETeamAttitude::Hostile; // 적 관계
    }

    return ETeamAttitude::Friendly; // 아군 관계
}

void UBSGameInstance::Init()
{
	Super::Init();

    FGenericTeamId::SetAttitudeSolver(&BSGetTeamAttitude);
}

void UBSGameInstance::Shutdown()
{
	UBSInventoryMenuWidget::CurrentTabIndex = 1;

	Super::Shutdown();
}
