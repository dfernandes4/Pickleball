// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownUserWidget.h"


#include "EnemyAIController.h"
#include "MainGamemode.h"
#include "UserWidgetLoader.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


class AMainPlayerController;

void UCountDownUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->EnableInput(PlayerController);
	
	CurrentCount = 3;
	PlayCountDownAnimation();
	
}

void UCountDownUserWidget::PlayCountDownAnimation()
{
	if(CurrentCount > 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CountDownSoundEffect);
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

		UGameplayStatics::PlaySound2D(GetWorld(), CountDownSoundKickoffEffect);
		float CountDownKickOffEffectDuration = CountDownSoundKickoffEffect->Duration;
		FTimerHandle KickOffTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(KickOffTimerHandle, [this]()
		{
			AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyAIController::StaticClass()));
			EnemyAIController->StartBehaviorTree();
			AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());
			MainGamemode->OnCountdownKickoffFinished.Broadcast();
		}, CountDownKickOffEffectDuration, false);
		
		const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
		WidgetLoader->LoadWidget(FName("PlayScreen"), GetWorld());
		
		
	}
}
