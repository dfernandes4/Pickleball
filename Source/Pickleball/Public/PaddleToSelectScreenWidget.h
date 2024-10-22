// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaddleToSelectScreenWidget.generated.h"

class UPaddleToCollectWidget;
/**
 * 
 */


UCLASS()
class PICKLEBALL_API UPaddleToSelectScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnUseButtonClicked();
	
	UFUNCTION()
	void SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName);

	UPROPERTY()
	UPaddleToCollectWidget* PaddleToCollectWidgetSelected;
	
private:

	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = Images, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> BackgroundImage;

	UPROPERTY(EditAnywhere, Category = Image, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> PaddleToSelectImage;

	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(EditAnywhere, Category = Buttons, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> UseButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SettingsHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> PaddleNameTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* MenuSoundEffect;
};
