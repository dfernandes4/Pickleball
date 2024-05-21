// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopScreenWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopClosedDelegate);

UCLASS()
class PICKLEBALL_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnBackButtonClicked();
	
	UPROPERTY()
	FOnShopClosedDelegate OnShopClosed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* BackSoundEffect;

private:

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> InternalCanvasPanel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton>	BackButton;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackroundImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> CommonImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> RareImage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> EpicImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> LegendaryImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> CommonTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> RareTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> EpicTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> LegendaryTextBlock;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UScrollBox> ScrollBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWrapBox> CommonWrapBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> RareWrapBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> EpicWrapBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> LegendaryWrapBox;

	
};
