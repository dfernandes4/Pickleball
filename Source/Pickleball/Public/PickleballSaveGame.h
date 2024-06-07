﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "GameFramework/SaveGame.h"
#include "PickleballSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPickleballSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPickleballSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData, meta = (AllowPrivateAccess))
	FPlayerData PlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData, meta = (AllowPrivateAccess))
	int32 EnemyLastRow;
};
