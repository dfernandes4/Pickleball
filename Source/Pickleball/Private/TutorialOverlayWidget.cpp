// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialOverlayWidget.h"

#include "MainPlayerController.h"
#include "UserWidgetLoader.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"

void UTutorialOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AnimCount = 0;
	OverlayButton->SetIsEnabled(false);
	OverlayButton->OnClicked.AddDynamic(this, &UTutorialOverlayWidget::OnOverlayButtonClicked);
	
	PlayAnimation(WelcomeFadeInFadeOutAnimation);

	const float AnimationDuration = WelcomeFadeInFadeOutAnimation->GetEndTime();
	FTimerHandle WelcomeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(WelcomeTimerHandle, [this]()
	{
		OnOverlayButtonClicked();
	}, AnimationDuration, false);
}

void UTutorialOverlayWidget::OnOverlayButtonClicked()
{
	OverlayButton->SetIsEnabled(false);

	if(AnimCount == 0)
	{
		TransitionToNextAnim(WhereToHitToFadeInAnimation);
		PlayAnimation(SlideBubblesFadeInAnimation);
	}
	else if(AnimCount == 1)
	{
		TransitionToNextAnim(KitchenFadeInAnimation, WhereToHitToFadeOutAnimation);
		PlayAnimation(PickleballHop1Animation);
	}
	else if(AnimCount == 2)
	{
		TransitionToNextAnim(KitchenText2FadeInAnimation, KitchenText1FadeOutAnimation);
		PlayAnimation(PickleballHop2Animation);	
	}
	else if(AnimCount == 3)
	{
		TransitionToNextAnim(WhereToHitFromFadeInAnimation, KitchenText2FadeOutAnimation);
		PlayAnimation(PickleballHop3Animation);
	}
	else if(AnimCount == 4)
	{
		TransitionToNextAnim(SwipeToControlFadeInAnimation, WhereToHitFromFadeOutAnimation);
		PlayAnimation(PickleballHop4Animation);

		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->EnableInput(PlayerController);
	}
	else if(AnimCount == 5)
	{
		PlayAnimation(SwipeToControlFadeOutAnimation);
		PlayAnimation(SlideBubblesFadeOutAnimation);
		
		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->EnableInput(PlayerController);
		
		const float SwipeToControlFadeOutAnimationDuration = SwipeToControlFadeOutAnimation->GetEndTime();
		
		FTimerHandle SwipeToControlFadeOutTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SwipeToControlFadeOutTimerHandle, [this]()
		{
			const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
			WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
		}, SwipeToControlFadeOutAnimationDuration, false);
	}
}

void UTutorialOverlayWidget::TransitionToNextAnim(UWidgetAnimation* FadeInAnimOfNext, UWidgetAnimation* FadeOutAnimOfCurrent)
{
	if(FadeOutAnimOfCurrent != nullptr)
	{
		PlayAnimation(FadeOutAnimOfCurrent);
		const float FadeOutAnimOfCurrentAnimationDuration = FadeOutAnimOfCurrent->GetEndTime();
		
		FTimerHandle WhereToHitToFadeInTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(WhereToHitToFadeInTimerHandle, [this, FadeInAnimOfNext]()
		{
			TransitionToNextAnim(FadeInAnimOfNext);
		}, FadeOutAnimOfCurrentAnimationDuration, false);
	}
	else
	{
		PlayAnimation(FadeInAnimOfNext);
		const float FadeInAnimOfNextAnimationDuration = FadeInAnimOfNext->GetEndTime();
		
		FTimerHandle FadeInAnimOfNextTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FadeInAnimOfNextTimerHandle, [this]()
		{
			AnimCount++;
			OverlayButton->SetIsEnabled(true);
		}, FadeInAnimOfNextAnimationDuration, false);
	}
}
