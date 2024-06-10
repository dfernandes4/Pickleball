// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayScreenWidget.h"

#include "Ball.h"
#include "EnemyPaddle.h"
#include "MainGamemode.h"
#include "PickleBallGameInstance.h"
#include "PlayerPaddle.h"
#include "SettingScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	if(MainGamemode)
	{
		MainGamemode->OnScoreUpdated.AddDynamic(this, &UPlayScreenWidget::UpdateScore);
		MainGamemode->OnGameOver.AddDynamic(this, &UPlayScreenWidget::HandleGameOver);
	}
	if(PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UPlayScreenWidget::PauseButtonClicked);
	}
	
	if(ScoreText != nullptr )
	{
		APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if(PlayerPaddle != nullptr)
		{
			ScoreText->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetCurrentScore())));
		}
	}
	UPickleBallGameInstance* PickleBallGameInstance = Cast<UPickleBallGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(!PickleBallGameInstance->GetIsFirstTimePlaying())
	{
		SetRandomEnemyAttributes();
	}
	
	PlayAnimation(HUDSlideInAnimation);	
}

void UPlayScreenWidget::UpdateScore(int NewScore)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(NewScore))); 

	UGameplayStatics::PlaySound2D(GetWorld(), PointSoundEffect);
}

void UPlayScreenWidget::PauseButtonClicked()
{
	bool bIsCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UGameplayStatics::SetGamePaused(GetWorld(), !bIsCurrentlyPaused);

	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	USettingScreenWidget* SettingScreenWidget = Cast<USettingScreenWidget>(WidgetLoader->LoadWidget(FName("SettingScreen"), GetWorld(),  1));
	SettingScreenWidget->OnSettingsClosed.AddDynamic(this, &UPlayScreenWidget::HandlePauseClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), PauseSoundEffect);
}

void UPlayScreenWidget::HandleGameOver()
{
	// Could play a fade out of everything thing then delete screen
	RemoveFromParent();


	ABall* Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	
	if(Ball != nullptr)
	{
		if(!Ball->CanBallHitInKitchen())
		{
			const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
			WidgetLoader->LoadWidget(FName("KitchenScreen"), GetWorld());
		}
		else
		{
			// Should play an Out or Bounced Twice Screen like kitchen
			const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
			WidgetLoader->LoadWidget(FName("FaultWidgetScreen"), GetWorld());
		}
	}

	MainGamemode->OnGameOver.RemoveDynamic(this, &UPlayScreenWidget::HandleGameOver);
}

void UPlayScreenWidget::SetRandomEnemyAttributes()
{
	 AEnemyPaddle* EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPaddle::StaticClass()));
	if(EnemyPaddle->CurrentEnemyAttributes != nullptr)
	{
		EnemyNameText->SetText(FText::FromString(EnemyPaddle->CurrentEnemyAttributes->EnemyName));
		EnemyIconImage->SetBrushFromTexture(EnemyPaddle->CurrentEnemyAttributes->EnemyIconTexture);
	}
}

void UPlayScreenWidget::HandlePauseClosed()
{
	SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
