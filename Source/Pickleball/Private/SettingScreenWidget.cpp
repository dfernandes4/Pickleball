// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingScreenWidget.h"

#include "MainGamemode.h"
#include "MainPlayerController.h"
#include "PickleBallGameInstance.h"
#include "UserWidgetLoader.h"
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
	
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &USettingScreenWidget::OnBackButtonClicked);
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

	if(HowToPlayButton != nullptr)
	{
		HowToPlayButton->OnClicked.AddDynamic(this, &USettingScreenWidget::OnHowToPlayButtonClicked);
	}

	if(RemoveAdsButton != nullptr)
	{
		RemoveAdsButton->OnClicked.AddDynamic(this, &USettingScreenWidget::OnRemoveAdsButtonClicked);
	}
	
	// if home is active or not in game then this button is active if not then it is disabled
	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGamemode->bIsGameActive ? HowToPlayButton->SetIsEnabled(false) : HowToPlayButton->SetIsEnabled(true);
    
    UPickleBallGameInstance* PickleBallGameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
    if(PickleBallGameInstance != nullptr)
    {
        if(!PickleBallGameInstance->AreAdsEnabled())
        {
            RemoveAdsButton->SetIsEnabled(false);
            RestorePurchasesButton->SetIsEnabled(false);
        }
        PickleBallGameInstance->AdsRemoved.AddDynamic(this, &USettingScreenWidget::OnAdsRemoved);
    }
}
	
void USettingScreenWidget::OnHowToPlayButtonClicked()
{
	OnHowToPlayButtonClickedDelegate.Broadcast();
	RemoveFromParent();
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("TutorialScreen"), GetWorld());	
}


void USettingScreenWidget::OnRemoveAdsButtonClicked()
{
	Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->InitiatePurchaseRequest("RemoveAds");
}

void USettingScreenWidget::OnBackButtonClicked()
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

void USettingScreenWidget::OnAdsRemoved()
{
    RemoveAdsButton->SetIsEnabled(false);
}
