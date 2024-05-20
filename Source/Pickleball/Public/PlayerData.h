#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

#pragma region PlayerStats
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
	int32 PlayerCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
	int32 PlayerHighScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
	int32 PlayersLastScore;

	// String : Paddle ID In DT, bool : Unlock Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
	TMap<FString, bool> PaddleUnlockStatuses;

#pragma endregion PlayerStats

// Volume Settings
#pragma region Settings
#pragma endregion Settings
};
