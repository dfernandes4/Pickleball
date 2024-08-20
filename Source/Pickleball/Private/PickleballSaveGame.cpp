// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleballSaveGame.h"

UPickleballSaveGame::UPickleballSaveGame() : PlayerData()
{
	EnemyLastRow = 10;
    bAreAdsEnabled = true;
	Volumes.Init(1.0f, 3);
}
