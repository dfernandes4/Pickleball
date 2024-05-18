// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KitchenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UKitchenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void PlayKitchenTextAnimation();

	UFUNCTION()
	void KitchenTextTimerFinished();
	
private:
	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	UPROPERTY(EditAnywhere, Category = Text, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> KitchenTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetAnimation> KitchenTextAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EASTEER", meta = (AllowPrivateAccess = "true"))
	USoundBase* WhistleSoundEffect;
};
