// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenWidget.h"

#include "UserWidgetLoader.h"
#include "Animation/WidgetAnimation.h"

void UKitchenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayKitchenTextAnimation();
}

void UKitchenWidget::PlayKitchenTextAnimation()
{
	PlayAnimation(KitchenTextAnimation);
	// Play Whistle Sound
	
	const float AnimationDuration = KitchenTextAnimation->GetEndTime();
	FTimerHandle CountdownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UKitchenWidget::KitchenTextTimerFinished, AnimationDuration, false);
}

void UKitchenWidget::KitchenTextTimerFinished() 
{
	RemoveFromParent();
	
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("GameOverScreen"), GetWorld());
}
