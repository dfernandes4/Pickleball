// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameInstance.h"

#include "IOSSaveGameSystem.h"
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
        TArray<uint8> Data;
        // Binary Load
        if (UGameplayStatics::LoadDataFromSlot(Data, SlotName, 0))
        {
            if(Data.IsEmpty())
            {
                
                ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();
                if (SaveSystem)
                {
                    FIOSSaveGameSystem* IOSSaveSystem = static_cast<FIOSSaveGameSystem*>(SaveSystem);
                    if (IOSSaveSystem != nullptr && IOSSaveSystem->OnUpdateLocalSaveFileFromCloud.IsBound())
                    {
                        IOSSaveSystem->OnUpdateLocalSaveFileFromCloud.BindUObject(this, &UPickleBallGameInstance::OnCloudLoadCompleted);
                    }
                }
                
            }
            else
            {
                SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromMemory(Data));
                if(SaveGame)
                {
                    bIsGameLoaded = true;
                    LoadFinished.Broadcast();
                }
            }
        }
        
        bIsFirstTimePlayingEver = false;
        
        //Sync Reg
        
        /*
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        if(SaveGame != nullptr)
        {
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
        }
         */
        
        // Async Implementation
        
        /*
        FAsyncLoadGameFromSlotDelegate LoadedDelegate;
        LoadedDelegate.BindUObject(this, &UPickleBallGameInstance::OnLoadFinished);

        // Initiate the async load for the save game
        UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
         */
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
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
            return true; // Operation was successful
        }
    }
    return false; // Operation was not successful
}

void UPickleBallGameInstance::SaveGameData()
{
    if(UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("file"), 0))
    {
        UE_LOG(LogTemp, Log, TEXT("Game Saved Successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Game Didn't Save Successfully"));
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
            bIsGameLoaded = true;
            LoadFinished.Broadcast();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to cast LoadedGame to UPickleballSaveGame"));
        }
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
