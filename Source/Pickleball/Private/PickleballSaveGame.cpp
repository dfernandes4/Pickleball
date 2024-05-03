// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleballSaveGame.h"

UPickleballSaveGame::UPickleballSaveGame()
{
	PlayerCoins = 0;
	PlayerHighScore = 0;
	PaddleUnlockStatuses.Add("BasePaddle", true);
}

int32 UPickleballSaveGame::GetPlayerCoins() const
{
	return PlayerCoins;
}

int32 UPickleballSaveGame::GetPlayerHighScore() const
{
	return PlayerHighScore;
}

TMap<FString, bool> UPickleballSaveGame::GetPaddleUnlockStatuses() const
{
	return PaddleUnlockStatuses;
}

void UPickleballSaveGame::SetPlayerCoins(int32 CoinsAmount)
{
	PlayerCoins = CoinsAmount;
}

void UPickleballSaveGame::SetPlayerHighScore(int32 HighScoreAmount)
{
	PlayerHighScore = HighScoreAmount;
}

void UPickleballSaveGame::SetPaddleUnlockStatuses(const TMap<FString, bool>& PaddleUnlockStatusesIn)
{
	PaddleUnlockStatuses = PaddleUnlockStatusesIn;
}
