// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaddleToCollectWidget.generated.h"

/**
 * 
 */

class UImage;

UCLASS()
class PICKLEBALL_API UPaddleToCollectWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnPaddleButtonClicked();

public:
	UFUNCTION()
	void SetPaddleAttributes(bool bIsPaddleUnlocked);
	
	TTuple<UObject*, const FVector2D&> GetPaddleImageInfo();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Image", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> CheckImage;
private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Canvas", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> Canvas;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SizeBox", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> PaddleSizeBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Button", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PaddleToCollectBtn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* MenuSoundEffect;
};
