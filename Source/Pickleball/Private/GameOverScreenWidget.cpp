// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreenWidget.h"

#include "HomeScreenWidget.h"
#include "PickleBallGameInstance.h"
#include "PlayerPaddle.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UGameOverScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::PlaySound2D(GetWorld(),GameoverSoundEffect);
	
	if(ReplayButton!= nullptr)
	{
		ReplayButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnReplayButtonClicked);
	}

	if(HomeButton!= nullptr)
	{
		HomeButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnHomeButtonClicked);
	}

	if(WatchAdd2xCoinsButton!= nullptr)
	{
		WatchAdd2xCoinsButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnWatchAdd2xCoinsButtonClicked);
	}

	if(WatchAddContinueButton!= nullptr)
	{
		WatchAddContinueButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnWatchAddContinueButtonClicked);
	}

	PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	DisplayPlayerValues();
}

void UGameOverScreenWidget::OnReplayButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	PlayerPaddle->SetCurrentScore(0);

	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
	GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UGameOverScreenWidget::OnHomeButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	Cast<UPickleBallGameInstance>(GetGameInstance())->SetShouldLaunchStarterScreen(true);
	PlayerPaddle->SetCurrentScore(0);
	
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
	GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	RemoveFromParent();
}

void UGameOverScreenWidget::OnWatchAdd2xCoinsButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	// Create Timer, after ad then do this
	PlayerPaddle->AddCoins(PlayerPaddle->GetCoinsEarnedFromLastMatch());
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
	GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
	
	// Need to make this a one time use per match, turn grey after used
}

void UGameOverScreenWidget::OnWatchAddContinueButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	// Create Timer, after ad then do this
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UGameOverScreenWidget::HandleChildClosed()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UGameOverScreenWidget::DisplayPlayerValues()
{
	if (PlayerPaddle)
	{
		if(HighScoreAmountTextBlock != nullptr )
		{
			HighScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetHighScore())));
		}

		if(ScoreAmountTextBlock != nullptr)
		{
			ScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetScore())));
		}
		
		if(CoinsEarnedAmountTextBlock != nullptr)
		{
			CoinsEarnedAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetCoinsEarnedFromLastMatch())));
		}
	}
}



