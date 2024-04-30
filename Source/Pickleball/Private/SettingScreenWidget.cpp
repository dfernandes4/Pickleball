// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingScreenWidget.h"
#include "Sound/SoundClass.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USettingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(MasterSoundClass != nullptr)
	{
		MasterSlider->SetValue(MasterSoundClass->Properties.Volume);
	}
	if(SFXSoundClass != nullptr)
	{
		SfxSlider->SetValue(SFXSoundClass->Properties.Volume);
	}
	if(MusicSoundClass != nullptr)
	{
		MusicSlider->SetValue(MusicSoundClass->Properties.Volume);
	}
	
	if(ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &USettingScreenWidget::OnExitButtonClicked);
	}
	if(MasterSlider != nullptr)
	{
		MasterSlider->OnValueChanged.AddDynamic(this, &USettingScreenWidget::OnMasterVolumeChanged);
	}

	if(SfxSlider != nullptr)
	{
		SfxSlider->OnValueChanged.AddDynamic(this, &USettingScreenWidget::OnSFXVolumeChanged);
	}

	if(MusicSlider != nullptr)
	{
		MusicSlider->OnValueChanged.AddDynamic(this, &USettingScreenWidget::OnMusicVolumeChanged);
	}

	
}

void USettingScreenWidget::OnExitButtonClicked()
{
	RemoveFromParent();
	OnSettingsClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void USettingScreenWidget::OnMasterVolumeChanged(float NewVolume)
{
	if(MasterSoundClass != nullptr)
	{
		MasterSoundClass->Properties.Volume = NewVolume;
	}
}

void USettingScreenWidget::OnSFXVolumeChanged(float NewVolume)
{
	if(SFXSoundClass != nullptr)
	{
		SFXSoundClass->Properties.Volume = NewVolume;
	}
}

void USettingScreenWidget::OnMusicVolumeChanged(float NewVolume)
{
	if(MusicSoundClass != nullptr)
	{
		MusicSoundClass->Properties.Volume = NewVolume;
	}
}
