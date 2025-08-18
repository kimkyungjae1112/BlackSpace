// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSGameInstance.h"
#include "GenericTeamAgentInterface.h"
#include "MoviePlayer.h"                 // GetMoviePlayer, FLoadingScreenAttributes
#include "Widgets/SWeakWidget.h"         // SNew(SWeakWidget)
#include "Blueprint/UserWidget.h"

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

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::HandlePreLoadMap);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandlePostLoadMap);

    FGenericTeamId::SetAttitudeSolver(&BSGetTeamAttitude);
}

void UBSGameInstance::Shutdown()
{
	UBSInventoryMenuWidget::CurrentTabIndex = 1;

	Super::Shutdown();
}

void UBSGameInstance::ShowLoadingScreen(float MinDisplayTime)
{
    UClass* WidgetClass = LoadingScreenClass.LoadSynchronous();
    if (!WidgetClass)
    {
        return;
    }

    ActiveLoadingWidget = CreateWidget<UUserWidget>(this, WidgetClass);
    if (!ActiveLoadingWidget) 
    {
        return;
    }

    TSharedRef<SWidget> SlateWidget = ActiveLoadingWidget->TakeWidget();

    FLoadingScreenAttributes Attr;
    Attr.MinimumLoadingScreenDisplayTime = MinDisplayTime; 
    Attr.bAutoCompleteWhenLoadingCompletes = false;        
    Attr.bWaitForManualStop = true;                        
    Attr.WidgetLoadingScreen = SlateWidget;                

    // 세팅 & 재생
    GetMoviePlayer()->SetupLoadingScreen(Attr);
    GetMoviePlayer()->PlayMovie();
}

void UBSGameInstance::HideLoadingScreen()
{
    if (GetMoviePlayer()->IsMovieCurrentlyPlaying())
    {
        GetMoviePlayer()->StopMovie();
    }

    // 월드가 바뀌면 UMG는 쓸 일 없으니 정리
    ActiveLoadingWidget = nullptr;
}

void UBSGameInstance::HandlePreLoadMap(const FString& MapName)
{
    ShowLoadingScreen(0.5f);
}

void UBSGameInstance::HandlePostLoadMap(UWorld* LoadedWorld)
{
    HideLoadingScreen();
}
