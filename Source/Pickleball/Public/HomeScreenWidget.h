// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomeScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UHomeScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnPlayButtonClicked();
	
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> TitleTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> HighScoreTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> HighScoreAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PlayButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> SettingsButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ShopButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CollectionButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox>  HorizontalBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage>  PaddleLeft;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage>  PaddleMiddle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TitleHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage>  PaddleRight;

	
	

	

	
	                                                                                    
	
};


