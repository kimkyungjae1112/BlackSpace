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

    if (MusicComponent && MusicComponent->IsActive())
    {
        MusicComponent->Stop();
    }

    MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MusicSound, 1.0f, 1.0f, 0.0f, nullptr, bLoop, false);
}

void UBSAudioManagerSubsystem::StopMusic()
{
    if (MusicComponent && MusicComponent->IsActive())
    {
        MusicComponent->FadeOut(2.f, 0);
    }
}

void UBSAudioManagerSubsystem::SetMasterVolume(float Volume)
{
    if (!MasterSoundClass || !GetWorld()) return;

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
