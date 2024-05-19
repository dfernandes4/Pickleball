// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UTutorialOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnOverlayButtonClicked();

	UFUNCTION()
	void TransitionToNextAnim(UWidgetAnimation* FadeInAnimOfNext, UWidgetAnimation* FadeOutAnimOfCurrent = nullptr);
	
private:

#pragma region Properties
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> WhereToHitToImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> WhereToHitFromImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> KitchenZoneImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> SlideBubblesImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> PickleballIconImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> WhereToHitToTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> WhereToHitFromTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> KitchenZoneTextBlock1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> KitchenZoneTextBlock2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> SwipeToHitTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> OverlayButton;

#pragma endregion Properties

#pragma region Animation

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WelcomeFadeInFadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WhereToHitToFadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WhereToHitToFadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KitchenFadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KitchenText1FadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KitchenText2FadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KitchenText2FadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WhereToHitFromFadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WhereToHitFromFadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SwipeToControlFadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SwipeToControlFadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PickleballHop1Animation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PickleballHop2Animation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PickleballHop3Animation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PickleballHop4Animation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SlideBubblesFadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SlideBubblesFadeOutAnimation;
	
#pragma endregion Animation

#pragma region ClassVariables

	UPROPERTY()
	int32 AnimCount;
	
#pragma endregion ClassVariables
};
