// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownUserWidget.h"

#include "UserWidgetLoader.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UCountDownUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentCount = 3;
	PlayCountDownAnimation();
}

void UCountDownUserWidget::PlayCountDownAnimation()
{
	if(CurrentCount > 0)
	{
		CountDownTextBlock->SetText(FText::FromString(FString::FromInt(CurrentCount)));
		PlayAnimation(FadeInAnimation);

		// Set a timer for the duration of the animation
		const float AnimationDuration = FadeInAnimation->GetEndTime();
		FTimerHandle CountdownTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UCountDownUserWidget::CountdownTimerFinished, AnimationDuration, false);
	}
}

void UCountDownUserWidget::CountdownTimerFinished()
{
	CurrentCount--;
	if(CurrentCount > 0)
	{
		PlayCountDownAnimation();
	}
	else
	{
		RemoveFromParent();

		const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
		WidgetLoader->LoadWidget(FName("PlayScreen"), GetWorld());
		
		APlayerController* PlayerController =  GetWorld()->GetFirstPlayerController();
		PlayerController->EnableInput(PlayerController);
	}
}
