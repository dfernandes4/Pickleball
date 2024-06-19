// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaddleToCollectWidget.h"
#include "Blueprint/UserWidget.h"
#include "HomeScreenWidget.generated.h"

class UShopScreenWidget;
class UCollectionWidget;
/**
 * 
 */
UCLASS()
class PICKLEBALL_API UHomeScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPlayButtonClicked();
	UFUNCTION()
	void OnHowToPlayButtonClicked();
	UFUNCTION()
	void OnSettingsButtonClicked();
	UFUNCTION()
	void DisplayPaddles(UPaddleToCollectWidget* PaddleSelected, UPaddleToCollectWidget* PaddleBefore, UPaddleToCollectWidget* PaddleAfter);
	UFUNCTION()
	void OnCollectionButtonClicked();
	UFUNCTION()
	void OnShopButtonClicked();
	UFUNCTION()
	void OnPlusCoinClicked();
	UFUNCTION()
	void OnLeftArrowClicked();
	UFUNCTION()
	void OnRightArrowClicked();
	UFUNCTION()
	void OnLeaderboardButtonClicked();
	
	UFUNCTION()
	void HandleChildClosed();
	
	UFUNCTION()
	void DisplayPlayerValues();
	UFUNCTION()
	void DisplayBasePaddles();

	UFUNCTION()
	void SetUpCollectionDelegates();

	UFUNCTION()
	void SetUpShopDelegates();

	UFUNCTION()
	void UpdateCoins(int32 NewCoinAmount);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* MenuSoundEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCollectionWidget> CollectionWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UShopScreenWidget> ShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HomeScreenMusic;

	UPROPERTY()
	UAudioComponent* HomeScreenMusicComponent;

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> HighScoreAmountTextBlock;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CoinAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PlayButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> SettingsButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ShopButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CollectionButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> PlusCoinButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> LeftArrowButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> RightArrowButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> LeaderboardButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox>  HorizontalBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage>  PaddleLeft;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage>  PaddleMiddle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage>  PaddleRight;


	
};


