// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UCountDownUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void PlayCountDownAnimation();
	UFUNCTION()
	void CountdownTimerFinished();
	
private:
	UPROPERTY(EditAnywhere, Category = Panels, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	UPROPERTY(EditAnywhere, Category = Text, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> CountDownTextBlock;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnimation;
	
	int CurrentCount;
};
