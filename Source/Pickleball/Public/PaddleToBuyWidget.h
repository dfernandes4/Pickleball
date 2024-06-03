// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaddleRarity.h"
#include "Blueprint/UserWidget.h"
#include "PaddleToBuyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPaddleToBuyWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnPaddleButtonClicked();

public:
	
	void SetPaddleAttributes(bool bIsUnlocked);

private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	EPaddleRarity PaddleRarity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> Canvas;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PaddleToBuyBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Image", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> CoinImage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> PaddleAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> PaddleSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* MenuSoundEffect;
};
