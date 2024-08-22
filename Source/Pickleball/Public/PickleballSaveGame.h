// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	FPlayerData PlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	int32 EnemyLastRow;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
    bool bAreAdsEnabled;

	// 0 = Master, 1 = Music, 2 = SFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	TArray<float> Volumes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	FString PlayerId;
};
