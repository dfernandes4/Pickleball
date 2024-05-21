// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "SettingScreenWidget.generated.h"

class UBackgroundBlur;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsClosedDelegate);

UCLASS()
class PICKLEBALL_API USettingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnBackButtonClicked();

	UPROPERTY()
	FOnSettingsClosedDelegate OnSettingsClosed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* BackSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MasterSoundClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MusicSoundClass;


private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BoxImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> OutlineImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USlider> MasterSlider;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlider> SfxSlider;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlider> MusicSlider;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> RemoveAdsButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;


	UFUNCTION(meta = (AllowPrivateAccess = "true"))
	void OnMasterVolumeChanged(float NewVolume);

	UFUNCTION(meta = (AllowPrivateAccess = "true"))
	void OnSFXVolumeChanged(float NewVolume);

	UFUNCTION(meta = (AllowPrivateAccess = "true"))
	void OnMusicVolumeChanged(float NewVolume);
	
	
	
	
	




	
};
