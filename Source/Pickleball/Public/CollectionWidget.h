// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CollectionWidget.generated.h"

/**
 * 
 */

class UBackgroundBlur;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectionClosedDelegate);

UCLASS()
class PICKLEBALL_API UCollectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnBackButtonClicked();
	
	FOnCollectionClosedDelegate OnCollectionClosed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* BackSoundEffect;

private:

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = Images, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackgroundImage;

	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UScrollBox> ScrollBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWrapBox> CollectedPaddlesWrapBox;
	
};
