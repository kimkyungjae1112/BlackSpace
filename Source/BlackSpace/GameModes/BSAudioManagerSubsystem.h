// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BSAudioManagerSubsystem.generated.h"

class USoundBase;
class UAudioComponent;
class USoundMix;
class USoundClass;

UCLASS()
class BLACKSPACE_API UBSAudioManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
    // 설정에서 사용할 사운드 믹스
    UPROPERTY()
    TObjectPtr<USoundMix> SettingsSoundMix;

    // 볼륨 조절의 대상이 될 사운드 클래스
    UPROPERTY()
    TObjectPtr<USoundClass> MasterSoundClass;

    UPROPERTY()
    TObjectPtr<USoundClass> MusicSoundClass;

    UPROPERTY()
    TObjectPtr<USoundClass> SfxSoundClass;

private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> MusicComponent;

public:
    UBSAudioManagerSubsystem();

    // 서브시스템이 처음 생성될 때 호출됩니다.
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
    // 특정 위치에서 사운드 효과를 한 번 재생합니다.
    void PlaySoundAtLocation(USoundBase* Sound, FVector Location);

    // 배경 음악을 재생합니다.
    void PlayMusic(USoundBase* MusicSound, bool bLoop = true);

    // 현재 재생 중인 배경 음악을 중지합니다.
    void StopMusic();

    // 마스터 볼륨을 설정합니다 (0.0 ~ 1.0)
    void SetMasterVolume(float Volume);

    // 음악 볼륨을 설정합니다 (0.0 ~ 1.0)
    void SetMusicVolume(float Volume);

    // 효과음 볼륨을 설정합니다 (0.0 ~ 1.0)
    void SetSfxVolume(float Volume);
};
