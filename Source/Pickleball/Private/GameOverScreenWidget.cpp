// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreenWidget.h"
#include "PickleBallGameInstance.h"
#include "PlayerPaddle.h"
#include "EnemyPaddle.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"

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

	EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPaddle::StaticClass()));
	PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	DisplayPlayerValues();
}

void UGameOverScreenWidget::OnReplayButtonClicked()
{
    UWorld* World = GetWorld();
    
    if(World)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
        PlayerPaddle->SetCurrentScore(0);
        EnemyPaddle->SetCurrentRow(0);

        UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
        GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
        GameInstance->SaveCurrentEnemyRow(0);
        
        FName CurrentWorldName(*World->GetName());
        UGameplayStatics::OpenLevel(this, CurrentWorldName, false);
    }
}

void UGameOverScreenWidget::OnHomeButtonClicked()
{
    // Ensure player paddle and enemy paddle data are valid before accessing them
    if (PlayerPaddle && EnemyPaddle)
    {
        PlayerPaddle->SetCurrentScore(0);
        EnemyPaddle->SetCurrentRow(0);
        
        // Save player and enemy data
        UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
        if (GameInstance)
        {
            GameInstance->SaveCurrentEnemyRow(0);
            GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
            GameInstance->SetShouldLaunchStarterScreen(true);
        }
    }

    UWorld* World = GetWorld();
    if(World)
    {
        // Open the current level again
        FName CurrentWorldName(*World->GetName());
        UGameplayStatics::OpenLevel(GetWorld(), CurrentWorldName, false);
    }
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
	// Create Timer, after ad then do all this
	
	ISaveGameInterface* SaveGameInterface = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld()));
	SaveGameInterface->SaveCurrentEnemyRow(EnemyPaddle->GetCurrentRow());
	
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
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



