// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaddleToBuyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPaddleToBuyWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> Canvas;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PaddleToBuyBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Image", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> CoinImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Image", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> PaddleImage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> PaddleAmountTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> PaddleSizeBox;
};