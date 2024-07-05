// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UGameOverScreenWidget : public UUserWidget
{
	GENERATED_BODY()


virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReplayButtonClicked();

	UFUNCTION()
	void OnHomeButtonClicked();

	UFUNCTION()
	void OnWatchAdd2xCoinsButtonClicked();

	UFUNCTION()
	void OnWatchAddContinueButtonClicked();

	UFUNCTION()
	void HandleChildClosed();

	UFUNCTION()
	void DisplayPlayerValues();
    
    UFUNCTION(BlueprintCallable)
    void OnUserFinishedRewardAd();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* GameoverSoundEffect;
	
	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> HighScoreAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ScoreAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CoinsEarnedAmountTextBlock;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> ReplayButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> WatchAdd2xCoinsButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> WatchAddContinueButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> HomeButton;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackroundImage;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EASTEER", meta = (AllowPrivateAccess = "true"))
	USoundBase* MenuSoundEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class APlayerPaddle* PlayerPaddle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AEnemyPaddle* EnemyPaddle;

    UPROPERTY()
    bool bIs2xAd;
	
};
