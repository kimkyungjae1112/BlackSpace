// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSDamageNumberSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "UI/BSDamageNumberRootWidget.h"


UBSDamageNumberSubsystem::UBSDamageNumberSubsystem()
{
    static ConstructorHelpers::FClassFinder<UBSDamageNumberRootWidget> RootWidgetClassRef(TEXT("/Game/_Game/UI/Damage/WBP_DamageNumberRoot.WBP_DamageNumberRoot_C"));
    if (RootWidgetClassRef.Succeeded())
    {
        RootWidgetClass = RootWidgetClassRef.Class;
    }
}

void UBSDamageNumberSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    EnsureRootWidget();
}

void UBSDamageNumberSubsystem::Deinitialize()
{
    if (RootWidget)
    {
        RootWidget->RemoveFromParent();
        RootWidget = nullptr;
    }

    Active.Reset();

    Super::Deinitialize();
}

void UBSDamageNumberSubsystem::Tick(float DeltaTime)
{
    EnsureRootWidget();

    if (!RootWidget || Active.Num() == 0) 
    { 
        return; 
    }

    for (int32 i = Active.Num() - 1; i >= 0; --i)
    {
        UpdateItem(i, DeltaTime);
    }
}

void UBSDamageNumberSubsystem::AddDamage(const FVector& InWorldLoc, float InDamage, float LifeTime)
{
    EnsureRootWidget();
    if (!RootWidget) return; 

    FDamageItem Item;
    Item.WorldLoc = InWorldLoc;
    Item.Damage = InDamage;
    Item.Life = FMath::Max(0.1f, LifeTime);
    Item.Elapsed = 0.f;
    Item.PoolIndex = RootWidget->BorrowEntry(FText::AsNumber(FMath::RoundToInt(InDamage)));
    Active.Add(Item);
}

void UBSDamageNumberSubsystem::AddDamageAtActor(AActor* Target, float InDamage, FVector LocalOffset, float LifeTime)
{
    EnsureRootWidget();
    if (!RootWidget || !Target) return;

    FDamageItem Item;
    Item.TargetActor = Target;
    Item.LocalOffset = LocalOffset;
    Item.WorldLoc = Target->GetActorLocation() + LocalOffset; // 초기값
    Item.Damage = InDamage;
    Item.Life = FMath::Max(0.1f, LifeTime);
    Item.PoolIndex = RootWidget->BorrowEntry(FText::AsNumber(FMath::RoundToInt(InDamage)));
    Active.Add(Item);
}

void UBSDamageNumberSubsystem::AddDamageAtSocket(USkeletalMeshComponent* Mesh, FName Socket, float InDamage, FVector LocalOffset, float LifeTime)
{
    EnsureRootWidget();
    if (!RootWidget || !Mesh) return;

    FDamageItem Item;
    Item.TargetMesh = Mesh;
    Item.TargetSocket = Socket;
    Item.LocalOffset = LocalOffset;
    Item.WorldLoc = Mesh->DoesSocketExist(Socket) ? Mesh->GetSocketLocation(Socket) + LocalOffset : Mesh->GetComponentLocation() + LocalOffset;
    Item.Damage = InDamage;
    Item.Life = FMath::Max(0.1f, LifeTime);
    Item.PoolIndex = RootWidget->BorrowEntry(FText::AsNumber(FMath::RoundToInt(InDamage)));
    Active.Add(Item);
}

void UBSDamageNumberSubsystem::EnsureRootWidget()
{
    if (RootWidget || !GetWorld() || !RootWidgetClass) 
    { 
        return; 
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) 
    { 
        return; 
    }

    RootWidget = CreateWidget<UBSDamageNumberRootWidget>(PC, RootWidgetClass);
    if (RootWidget)
    {
        RootWidget->AddToViewport(10);   // ZOrder 넉넉히
        RootWidget->InitPool(64);        // 풀 초기 크기 (프로젝트에 맞게 조정)
    }
}

void UBSDamageNumberSubsystem::UpdateItem(int32 Index, float DeltaTime)
{
    FDamageItem& DamageItem = Active[Index];
    DamageItem.Elapsed += DeltaTime;
    const float T = FMath::Clamp(DamageItem.Elapsed / DamageItem.Life, 0.f, 1.f);

    // 타겟 추적(있으면 우선)
    if (DamageItem.TargetMesh.IsValid())
    {
        if (DamageItem.TargetSocket != NAME_None && DamageItem.TargetMesh->DoesSocketExist(DamageItem.TargetSocket))
        {
            DamageItem.WorldLoc = DamageItem.TargetMesh->GetSocketLocation(DamageItem.TargetSocket) + DamageItem.LocalOffset;
        }
        else
        {
            DamageItem.WorldLoc = DamageItem.TargetMesh->GetComponentLocation() + DamageItem.LocalOffset;
        }
    }
    else if (DamageItem.TargetActor.IsValid())
    {
        DamageItem.WorldLoc = DamageItem.TargetActor->GetActorLocation() + DamageItem.LocalOffset;
    }

    // 타겟 모두 소멸 시 조기 반환
    if (!DamageItem.TargetActor.IsValid() && !DamageItem.TargetMesh.IsValid() && DamageItem.PoolIndex != INDEX_NONE)
    {
        RootWidget->ReturnEntry(DamageItem.PoolIndex);
        Active.RemoveAtSwap(Index);
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    // 간단한 상승/흔들림
    const float Up = FMath::Lerp(0.f, 40.f, T);
    const float Sway = FMath::Sin(DamageItem.Elapsed * 12.f) * 6.f;
    const FVector World = DamageItem.WorldLoc + FVector(0, Sway, Up);

    // 화면 좌표 투영
    FVector2D ScreenPos;
    const bool bOnScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, World, ScreenPos, false);

    // 오클루전(선택)
    bool bVisible = bOnScreen;
    if (bUseOcclusionCheck && bOnScreen)
    {
        FHitResult Hit;
        FCollisionQueryParams Params(SCENE_QUERY_STAT(DamageNumberOcclusion), /*bTraceComplex=*/true);
        if (APawn* Pawn = PC->GetPawn()) { Params.AddIgnoredActor(Pawn); }
        const FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
        const bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, World, ECC_Visibility, Params);
        bVisible = !bBlocked || (Hit.GetActor() == DamageItem.TargetActor.Get());
    }

    // 알파/스케일
    const float Alpha = 1.f - T;
    const float Scale = 1.f + 0.15f * FMath::Sin(T * PI);

    if (DamageItem.PoolIndex != INDEX_NONE)
    {
        RootWidget->UpdateEntry(DamageItem.PoolIndex, ScreenPos, Alpha, Scale, bVisible);
    }

    if (DamageItem.Elapsed >= DamageItem.Life || !bOnScreen)
    {
        RootWidget->ReturnEntry(DamageItem.PoolIndex);
        Active.RemoveAtSwap(Index);
    }
}