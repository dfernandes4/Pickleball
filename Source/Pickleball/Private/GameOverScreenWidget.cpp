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
#include "UserWidgetLoader.h"

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
    
    bIs2xAd = true;
    
    UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
    if(GameInstance)
    {
        GameInstance->RewardFinished.AddDynamic(this, &UGameOverScreenWidget::OnUserFinishedRewardAd);
    }

	for (int i = 0; i < 3; i++)
	{
		GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
	}
    
}

void UGameOverScreenWidget::OnReplayButtonClicked()
{
    UWorld* World = GetWorld();
    
    if(World)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
        EnemyPaddle->SetCurrentRow(0);

        UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
        GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
        GameInstance->SaveCurrentEnemyRow(0);
        GameInstance->SetShouldLaunchStarterScreen(false);
        
        FName CurrentWorldName(*World->GetName());
        UGameplayStatics::OpenLevel(this, CurrentWorldName, false);
    }
}

void UGameOverScreenWidget::OnHomeButtonClicked()
{
    UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
    
    // Ensure player paddle and enemy paddle data are valid before accessing them
    if (PlayerPaddle && EnemyPaddle)
    {
        EnemyPaddle->SetCurrentRow(0);
        
        // Save player and enemy data
        UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
        if (GameInstance)
        {
            GameInstance->SavePlayerData(PlayerPaddle->GetCurrentPlayerData());
            GameInstance->SaveCurrentEnemyRow(0);
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
    bIs2xAd = true;
    UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
    UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
    if(GameInstance)
    {
        GameInstance->OnAdButtonPressed.Broadcast();
    }
}

void UGameOverScreenWidget::OnWatchAddContinueButtonClicked()
{
    bIs2xAd = false;
    UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
    UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
    if(GameInstance)
    {
        GameInstance->OnAdButtonPressed.Broadcast();
    }
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
			ScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetLastScore())));
		}
		
		if(CoinsEarnedAmountTextBlock != nullptr)
		{
			CoinsEarnedAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetCoinsEarnedFromLastMatch())));
		}
	}
}

void UGameOverScreenWidget::OnUserFinishedRewardAd()
{
    // Ensure that the following block of code runs on the Game Thread (main thread)
    AsyncTask(ENamedThreads::GameThread, [&]()
    {
        // Now we are guaranteed to be running on the Game Thread

        UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
        if (!GameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
            return;
        }

        if (bIs2xAd)
        {
            if (PlayerPaddle && EnemyPaddle)
            {
                // Safely interact with game objects on the main thread
                PlayerPaddle->AddCoins(PlayerPaddle->GetCoinsEarnedFromLastMatch());
                EnemyPaddle->SetCurrentRow(0);

                // Save player and enemy data on the main thread
                if (GameInstance)
                {
                    GameInstance->SaveCurrentEnemyRow(0);
                    GameInstance->SetShouldLaunchStarterScreen(true);

                    UWorld* World = GetWorld();
                    if(World)
                    {
                        // Open the current level again
                        FName CurrentWorldName(*World->GetName());
                        UGameplayStatics::OpenLevel(GetWorld(), CurrentWorldName, false);
                    }
                }
            }
        }
        else
        {
            if (PlayerPaddle && EnemyPaddle)
            {

                // Safely interact with game objects on the main thread
                PlayerPaddle->SaveLastScore();
                GameInstance->SaveCurrentEnemyRow(EnemyPaddle->GetCurrentRow());
                GameInstance->SetShouldLaunchStarterScreen(false);
            	GameInstance->SetIsContinueGame(true);
                
                UWorld* World = GetWorld();
                if(World)
                {
                    // Open the current level again
                    FName CurrentWorldName(*World->GetName());
                    UGameplayStatics::OpenLevel(GetWorld(), CurrentWorldName, false);
                }
            }
        }
    });
}

