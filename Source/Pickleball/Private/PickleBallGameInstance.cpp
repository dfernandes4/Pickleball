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
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        if (SaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("SaveGame loaded successfully"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load SaveGame"));
        }
        bIsFirstTimePlaying = false;
    }
    else
    {
        SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
        if (SaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("SaveGame created successfully"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create SaveGame"));
        }
        bIsFirstTimePlaying = true;
    }
}

void UPickleBallGameInstance::SaveGameData()
{
	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
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
	return bIsFirstTimePlaying;
}

void UPickleBallGameInstance::SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn)
{
	bShouldLaunchStarterScreen = bIShouldLaunchStarterScreenIn;
}

void UPickleBallGameInstance::SetIsFirstTimePlaying(bool bIsFirstTimePlayingIn)
{
	bIsFirstTimePlaying = bIsFirstTimePlayingIn;
}

int32 UPickleBallGameInstance::GetSaveGameEnemyRow()
{
    return SaveGame->EnemyLastRow;
}
 
bool UPickleBallGameInstance::GetShouldLaunchStarterScreen() const
{
	return bShouldLaunchStarterScreen;
}
