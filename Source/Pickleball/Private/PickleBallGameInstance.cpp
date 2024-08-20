// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameInstance.h"

#include "PickleballSaveGame.h"
#include "PlatformFeatures.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"


UPickleBallGameInstance::UPickleBallGameInstance()
{
	SlotName = "file";
    bIsFirstTimePlayingEver = true;
    bIsFirstTimePlayingInSession = true;
    bShouldLaunchStarterScreen = true;
    bIsGameLoaded = false;
    RetryCount = 0;
    MaxRetries = 3;
    RetryDelay = .2f;
    CurrentGameCount = 0;
}

void UPickleBallGameInstance::Init()
{
	Super::Init();

	LoadGameData();
}

void UPickleBallGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UPickleBallGameInstance::LoadGameData()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        bIsFirstTimePlayingEver = false;
        
        TArray<uint8> Data;
        // Binary Load
        if (UGameplayStatics::LoadDataFromSlot(Data, SlotName, 0))
        {
            if(Data.IsEmpty())
            {
                FTimerHandle CloudLoadTimerHandle;
                GetWorld()->GetTimerManager().SetTimer(CloudLoadTimerHandle, [this]()
                {
                    OnCloudLoadCompleted(FString(FString::Printf(TEXT("%s""SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), TEXT("file"))));
                }, 1.f, false);
            }
            else
            {
                SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromMemory(Data));
                if(SaveGame)
                {
                    SaveGame->PlayerData.PlayersLastScore = 0;
                    bIsGameLoaded = true;
                    LoadFinished.Broadcast();
                }
            }
        }
    }
    else
    {
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
        SaveGameData();
        bIsFirstTimePlayingEver = true;
        bIsGameLoaded = true;
        LoadFinished.Broadcast();
    }
    bIsFirstTimePlayingInSession = true;
}

bool UPickleBallGameInstance::OnCloudLoadCompleted(const FString& FileName)
{
    TArray<uint8> Data;
    FFileHelper::LoadFileToArray(Data, *FileName);
    if(!Data.IsEmpty())
    {
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromMemory(Data));
        if(SaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("Loaded save High Score: %d"), SaveGame->PlayerData.PlayerHighScore);
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
            return true; // Operation was successful
        }
    }
    return false; // Operation was not successful
}

void UPickleBallGameInstance::SaveGameData()
{
    // Serialize the SaveGame object to a byte array
    TArray<uint8> SaveData;
    if (UGameplayStatics::SaveGameToMemory(SaveGame, SaveData))
    {
        UGameplayStatics::SaveDataToSlot(SaveData, SlotName, 0);
        // Save the byte array to a local file
        FString LocalSavePath = FString::Printf(TEXT("%sSaveGames/%s.sav"), *FPaths::ProjectSavedDir(), TEXT("file"));
        if (FFileHelper::SaveArrayToFile(SaveData, *LocalSavePath))
        {
            UE_LOG(LogTemp, Log, TEXT("Game Saved Locally Successfully"));
            UE_LOG(LogTemp, Log, TEXT("Saved High Score: %d"), SaveGame->PlayerData.PlayerHighScore);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to Save Game Locally"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to Serialize SaveGame Object"));
    }
    
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

void UPickleBallGameInstance::RewardFinishedComplete() const
{
    RewardFinished.Broadcast();
}

TArray<int32> UPickleBallGameInstance::GetSaveGameVolumes()
{
    return SaveGame->Volumes;
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

int32 UPickleBallGameInstance::GetCurrentGameCount() const
{
    return CurrentGameCount;
}

void UPickleBallGameInstance::IncrementCurrentGameCount()
{
    CurrentGameCount++;
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

bool UPickleBallGameInstance::GetIsGameLoaded() const
{
    return bIsGameLoaded;
}
int32 UPickleBallGameInstance::GetSaveGameEnemyRow()
{
    return SaveGame->EnemyLastRow;
}

bool UPickleBallGameInstance::GetShouldLaunchStarterScreen() const
{
	return bShouldLaunchStarterScreen;
}

void UPickleBallGameInstance::RemoveAds()
{
    SaveGame->bAreAdsEnabled = false;
    SaveGameData();

    AdsRemoved.Broadcast();
}

bool UPickleBallGameInstance::AreAdsEnabled() const
{
    return SaveGame->bAreAdsEnabled;
}

void UPickleBallGameInstance::SaveVolumes(float MasterVolume, float MusicVolume, float SFXVolume)
{
    SaveGame->Volumes[0] = FMath::Floor(MasterVolume);
    SaveGame->Volumes[1] = FMath::Floor(MusicVolume);
    SaveGame->Volumes[2] = FMath::Floor(SFXVolume);

    for (int i = 0; i < 3; i++)
    {
        SaveGameData();
    }
}
