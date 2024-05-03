// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION()
	int32 GetPlayerCoins() const;
	UFUNCTION()
	int32 GetPlayerHighScore() const;
	UFUNCTION()
	TMap<FString, bool> GetPaddleUnlockStatuses() const;

	UFUNCTION()
	void SetPlayerCoins(int32 CoinsAmount);
	UFUNCTION()
	void SetPlayerHighScore(int32 HighScoreAmount);
	UFUNCTION()
	void SetPaddleUnlockStatuses(const TMap<FString, bool>& PaddleUnlockStatusesIn);
	
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData, meta = (AllowPrivateAccess))
	int32 PlayerCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData, meta = (AllowPrivateAccess))
	int32 PlayerHighScore;

	// String : Paddle ID In DT, bool : Unlock Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData, meta = (AllowPrivateAccess))
	TMap<FString, bool> PaddleUnlockStatuses;
};
