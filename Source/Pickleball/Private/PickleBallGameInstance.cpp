// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameInstance.h"

#include "MainPlayerController.h"
#include "OnlineSubsystem.h"
#include "PickleballSaveGame.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"


UPickleBallGameInstance::UPickleBallGameInstance()
{
	SlotName = "PlayerSaveSlot";
}

void UPickleBallGameInstance::Init()
{
	Super::Init();

	LoadGameData();
}

void UPickleBallGameInstance::Shutdown()
{
	Super::Shutdown();

	SaveGame->PlayerData.PlayersLastScore = 0;
	SaveGame->EnemyLastRow = 0;
	SaveGameData();
}

void UPickleBallGameInstance::LoadGameData()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		bIsFirstTimePlaying = false;
	}
	else
	{
		SaveGame = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
		bIsFirstTimePlaying = true;
	}
}

void UPickleBallGameInstance::SaveGameData()
{
	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

FPlayerData UPickleBallGameInstance::GetSaveGamePlayerData()
{
	return SaveGame->PlayerData;
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
