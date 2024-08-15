// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinShopScreen.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinShopClosedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPurchaseCompletedDelegate, int32, CoinsAmount);

UCLASS()
class PICKLEBALL_API UCoinShopScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnBackButtonPressed();

	/*
	UFUNCTION()
	void OnSomeGoldButtonPressed();

	UFUNCTION()
	void OnFistOGoldButtonPressed();
	
	UFUNCTION()
	void OnLotsOGoldButtonPressed();
	
	UFUNCTION()
	void OnPilesOGoldButtonPressed();
	*/
	
	FOnCoinShopClosedDelegate OnCoinShopClosed;
	FOnPurchaseCompletedDelegate OnPurchaseCompletedDelegate;

	UFUNCTION(BlueprintCallable)
	void OnPurchaseCompleted(const FString& ProductId);
	

private:

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackBoxImage;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> SomeGoldButton;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> FistOGoldButton;
	
	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> LotsOGoldButton;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> PilesOGoldButton;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* BackSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* CoinPurchasedSound;
	
};
