// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaddleRarity.h"
#include "Blueprint/UserWidget.h"
#include "PaddleToBuyScreenWidget.generated.h"

/**
 * 
 */

UCLASS()
class PICKLEBALL_API UPaddleToBuyScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBuyButtonClicked();
	
	UFUNCTION()
	void SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName, const EPaddleRarity& PaddleRarity);

	UFUNCTION()
	void SetBackgroundHidden();
	
private:

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = Images, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackgroundImage;

	UPROPERTY(EditAnywhere, Category = Image, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> PaddleToBuyImage;

	UPROPERTY(EditAnywhere, Category = Image, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> UnlockedStarImage;

	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BuyButton;
	
	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CloseScreenOverlayButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> PaddleNameTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> UnlockedPaddleNameTextBlock;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> UnlockedTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* MenuSoundEffect;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PaddlePurchaseAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PaddleFloatingAnimation;
	
};
