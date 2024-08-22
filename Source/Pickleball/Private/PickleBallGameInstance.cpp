// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameInstance.h"

#include "OnlineSubsystem.h"
#include "PickleballSaveGame.h"
#include "TimerManager.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"

UPickleBallGameInstance::UPickleBallGameInstance()
{
	SlotName = "file";
    bIsFirstTimePlayingEver = true;
    bIsFirstTimePlayingInSession = true;
    bShouldLaunchStarterScreen = true;
    bIsGameLoaded = false;
    CurrentGameCount = 0;
}

void UPickleBallGameInstance::Init()
{
	Super::Init();
    
    LoginToGameCenter();
}

void UPickleBallGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UPickleBallGameInstance::LoginToGameCenter()
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
        if (Identity.IsValid())
        {
            OnLoginCompleteDelegate = FOnLoginCompleteDelegate::CreateUObject(this, &UPickleBallGameInstance::OnLoginComplete);
            Identity->AddOnLoginCompleteDelegate_Handle(0, OnLoginCompleteDelegate);

            FOnlineAccountCredentials AccountCredentials;
            Identity->Login(0, AccountCredentials);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Identity interface is not valid"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem IOS is not available"));
    }
}

void UPickleBallGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
    const FString& Error)
{
    OnLoginCompleteDelegate.Unbind();
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
        if (Identity.IsValid())
        {
            if (bWasSuccessful)
            {
                CurrentUserId = UserId.ToString();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Login to Game Center failed: %s"), *Error);
            }
        }
    }

    bIsLoggedIn = bWasSuccessful;
    LoadGameData();
}

void UPickleBallGameInstance::LoadGameData()
{
    if(bIsLoggedIn)
    {
        // Try to load the save game from the cloud
        
        FString FileName = FString(FString::Printf(TEXT("%s""SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), TEXT("file")));
        // Check if a save FILE exists and if so sets save game to local save
        if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
        {
            SetupValidSaveGame(FileName, false);
        }
    
        TArray<uint8> Data;
        // Updates local save file from icloud or just sets Data with local file data,
        // returns true if cloud load is successful or if local file load is successful
        // returns false if neither are successful
        if (UGameplayStatics::LoadDataFromSlot(Data, SlotName, 0))
        {
            bIsFirstTimePlayingEver = false;
            //Cloud save successful
            if(Data.IsEmpty())
            {
                FTimerHandle CloudLoadTimerHandle;
                GetWorld()->GetTimerManager().SetTimer(CloudLoadTimerHandle, [this, FileName]()
                {
                    SetupValidSaveGame(FileName, true);
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
        else
        {
            //No save file exists so create a new one
            SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
            SaveGameData();
            bIsFirstTimePlayingEver = true;
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
        }
    }
    else
    {
        // No Account so just use local save system
        
        FString FileName = FString(FString::Printf(TEXT("%s""SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), TEXT("file")));

        // Check if a save FILE exists and if so sets save game to local save
        if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
        {
            SetupValidSaveGame(FileName, false);
        }
        else
        {
            //No save file exists so create a new one
            SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
            SaveGameData();
            bIsFirstTimePlayingEver = true;
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
        }
    }
    
    bIsFirstTimePlayingInSession = true;
}

bool UPickleBallGameInstance::SetupValidSaveGame(const FString& FileName, bool bIsLoadingCloudSave)
{
    TArray<uint8> Data;
    FFileHelper::LoadFileToArray(Data, *FileName);
    if(!Data.IsEmpty())
    {
        if(bIsLoadingCloudSave)
        {
            UPickleballSaveGame* CloudSaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromMemory(Data));
            if(CloudSaveGame != nullptr && SaveGame != nullptr)
            {
                // If cloud save is up-to-date or current user is different than last time
                // then update local save with cloud save
                
                /* !!Could be potential issue if cloud save is ahead, double check this!! */
                const bool bIsCloudSaveUpToDate = ShouldiCloudOverride(SaveGame, CloudSaveGame);
                if(bIsCloudSaveUpToDate || (CurrentUserId != SaveGame->PlayerId))
                {
                    SaveGame = CloudSaveGame;
                }
                
                SaveGameData();
                bIsGameLoaded = true;
                LoadFinished.Broadcast();
                    
                UE_LOG(LogTemp, Log, TEXT("Loaded save High Score: %d"), SaveGame->PlayerData.PlayerHighScore);
                return true;
            }
        }
        else
        {
            SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromMemory(Data));
            return true;
        }
    }
    return false;
}

bool UPickleBallGameInstance::ShouldiCloudOverride(const UPickleballSaveGame* LocalSaveGame,
    const UPickleballSaveGame* CloudSaveGame) const
{
    // Check if all paddles are up-to-date
    for (auto Paddle : LocalSaveGame->PlayerData.PaddleUnlockStatuses)
    {
        if (CloudSaveGame->PlayerData.PaddleUnlockStatuses[Paddle.Key] != Paddle.Value)
        {
            // Check which has the paddle unlocked if its cloud then override other wise dont
            return CloudSaveGame->PlayerData.PaddleUnlockStatuses[Paddle.Key];
        }
    }

    /* Check this */
    // Check everything else
    if(LocalSaveGame->PlayerData.PlayerCoins != CloudSaveGame->PlayerData.PlayerCoins)
    {
        return LocalSaveGame->PlayerData.PlayerCoins <= CloudSaveGame->PlayerData.PlayerCoins;
    }

    return LocalSaveGame->PlayerData.PlayerHighScore <= CloudSaveGame->PlayerData.PlayerHighScore;
}

void UPickleBallGameInstance::SaveGameData()
{
    if (!SaveGame)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGame object is null, cannot save data."));
        return;
    }
    
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

TArray<float> UPickleBallGameInstance::GetSaveGameVolumes()
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
    SaveGame->Volumes[0] = MasterVolume;
    SaveGame->Volumes[1] = MusicVolume;
    SaveGame->Volumes[2] = SFXVolume;

    for (int i = 0; i < 3; i++)
    {
        SaveGameData();
    }
}
