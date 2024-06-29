// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameInstance.h"

#include "MainPlayerController.h"
#include "OnlineSubsystem.h"
#include "PickleballSaveGame.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"


UPickleBallGameInstance::UPickleBallGameInstance()
{
	SlotName = "file";
    bIsFirstTimePlayingEver = true;
    bIsFirstTimePlayingInSession = true;
    bShouldLaunchStarterScreen = true;
    RetryCount = 0;
    MaxRetries = 3;
    RetryDelay = .2f;
}

void UPickleBallGameInstance::Init()
{
	Super::Init();

	LoadGameData();
}

void UPickleBallGameInstance::Shutdown()
{
	Super::Shutdown();

    if(SaveGame != nullptr)
    {
        SaveGame->PlayerData.PlayersLastScore = 0;
        SaveGame->EnemyLastRow = 0;
        SaveGameData();
    }
}

void UPickleBallGameInstance::LoadGameData()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        FAsyncLoadGameFromSlotDelegate LoadedDelegate;
        LoadedDelegate.BindUObject(this, &UPickleBallGameInstance::OnLoadFinished);

        // Initiate the async load for the save game
        UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
    }
    else
    {
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
        SaveGameData();
        bIsFirstTimePlayingEver = true;
        LoadFinished.Broadcast();
    }
    bIsFirstTimePlayingInSession = true;
}

void UPickleBallGameInstance::SaveGameData()
{
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("file"), 0);
}

FPlayerData UPickleBallGameInstance::GetSaveGamePlayerData()
{
    if (SaveGame)
    {
        UE_LOG(LogTemp, Log, TEXT("Returning PlayerData from SaveGame"));
        return SaveGame->PlayerData;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGame is null"));
        return FPlayerData();  // Return a default-constructed FPlayerData
    }
}

void UPickleBallGameInstance::SavePlayerData(FPlayerData PlayerData)
{
	SaveGame->PlayerData = PlayerData;
	SaveGameData();
}

void UPickleBallGameInstance::SaveCurrentEnemyRow(int32 EnemyRow)
{
	SaveGame->EnemyLastRow = EnemyRow;
	SaveGameData();
}

bool UPickleBallGameInstance::GetIsFirstTimePlaying() const
{
	return bIsFirstTimePlayingEver;
}
void UPickleBallGameInstance::SetIsFirstTimePlayingEver(bool bIsFirstTimePlayingEverIn)
{
    bIsFirstTimePlayingEver = bIsFirstTimePlayingEverIn;
}

void UPickleBallGameInstance::SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn)
{
	bShouldLaunchStarterScreen = bIShouldLaunchStarterScreenIn;
}

bool UPickleBallGameInstance::GetIsFirstTimePlayingInSession() const
{
    return bIsFirstTimePlayingInSession;
}

void UPickleBallGameInstance::SetIsFirstTimePlayingInSession(bool bIsFirstTimePlayingInSessionIn)
{
    bIsFirstTimePlayingInSession = bIsFirstTimePlayingInSessionIn;
}

int32 UPickleBallGameInstance::GetSaveGameEnemyRow()
{
    return SaveGame->EnemyLastRow;
}

bool UPickleBallGameInstance::GetShouldLaunchStarterScreen() const
{
	return bShouldLaunchStarterScreen;
}

void UPickleBallGameInstance::OnLoadFinished(const FString& SlotNameIn, const int32 UserIndex, USaveGame* LoadedGame)
{
    if (LoadedGame)
    {
        // Reset retry count
        RetryCount = 0;

        // Store the loaded save game
        SaveGame = Cast<UPickleballSaveGame>(LoadedGame);
        if (SaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("SaveGame loaded successfully"));
            SaveGameData();
            LoadFinished.Broadcast();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to cast LoadedGame to UPickleballSaveGame"));
        }

        bIsFirstTimePlayingEver = false;
    }
    
    if(LoadedGame == nullptr || SaveGame == nullptr)
    {
        // Handle load failure and retry if needed
        if (RetryCount < MaxRetries)
        {
            RetryCount++;
            UE_LOG(LogTemp, Warning, TEXT("Retrying load SaveGame: Attempt %d"), RetryCount);
            GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UPickleBallGameInstance::RetryLoad);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load SaveGame from slot: %s after %d attempts"), *SlotName, RetryCount);
            RetryCount = 0; // Reset retry count for future attempts
        }
    }
}

void UPickleBallGameInstance::RetryLoad()
{
    FTimerHandle RetryTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        RetryTimerHandle,[this]() { LoadGameData(); },
        RetryDelay, 
        false
    );
}
