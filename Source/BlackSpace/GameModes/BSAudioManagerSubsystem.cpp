// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSAudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundBase.h"

UBSAudioManagerSubsystem::UBSAudioManagerSubsystem()
{
}

void UBSAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    SettingsSoundMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/_Game/Sound/SM_SoundSetting.SM_SoundSetting'"));
    MasterSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/_Game/Sound/SC_Master.SC_Master'"));
    MusicSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/_Game/Sound/SC_Music.SC_Music'"));
    SfxSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/_Game/Sound/SC_SFX.SC_SFX'"));
}

void UBSAudioManagerSubsystem::PlaySoundAtLocation(USoundBase* Sound, FVector Location)
{
    if (Sound && GetWorld())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    }
}

void UBSAudioManagerSubsystem::PlayMusic(USoundBase* MusicSound, bool bLoop)
{
    if (!MusicSound || !GetWorld())
    {
        return;
    }

    // 이미 음악이 재생 중이라면 중지시킵니다.
    if (MusicComponent && MusicComponent->IsActive())
    {
        MusicComponent->Stop();
    }

    // 2D 사운드로 새로운 음악을 재생합니다. UI나 배경음악에 적합합니다.
    MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MusicSound, 1.0f, 1.0f, 0.0f, nullptr, false, bLoop);
}

void UBSAudioManagerSubsystem::StopMusic()
{
    if (MusicComponent && MusicComponent->IsActive())
    {
        MusicComponent->Stop();
    }
}

void UBSAudioManagerSubsystem::SetMasterVolume(float Volume)
{
    if (!MasterSoundClass || !GetWorld()) return;

    // 사운드 믹스 내에서 특정 사운드 클래스의 볼륨을 실시간으로 덮어씁니다.
    // FMath::Clamp를 사용하여 볼륨 값이 항상 0.0과 1.0 사이를 유지하도록 합니다.
    UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SettingsSoundMix, MasterSoundClass, FMath::Clamp(Volume, 0.0f, 1.0f));

}

void UBSAudioManagerSubsystem::SetMusicVolume(float Volume)
{
    if (!MusicSoundClass || !GetWorld()) return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SettingsSoundMix, MusicSoundClass, FMath::Clamp(Volume, 0.0f, 1.0f));
}

void UBSAudioManagerSubsystem::SetSfxVolume(float Volume)
{
    if (!SfxSoundClass || !GetWorld()) return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SettingsSoundMix, SfxSoundClass, FMath::Clamp(Volume, 0.0f, 1.0f));
}
