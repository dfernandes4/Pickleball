#pragma once

#include "CoreMinimal.h"
#include "PaddleToCollectWidget.h"
#include "PlayerData.generated.h"


USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

    FPlayerData() : PlayerCoins(0), PlayerHighScore(0), PlayersLastScore(0), CurrentPaddleName(FName("Green"))
    {
        PaddleUnlockStatuses = {
            // Common
            {FName("Green"), true},
            {FName("Orange"), false},
            {FName("Purple"), false},
            {FName("Red"), false},
            {FName("Yellow"), false},
            {FName("Blue"), false},
            {FName("Polka"), false},
            {FName("Pride"), false},
            // Rare
            {FName("Webbed"), false},
            {FName("Camo"), false},
            {FName("Lightning"), false},
            {FName("The_Patriot"), false},
            {FName("Pizza"), false},
            // Epic
            {FName("Doggy"), false},
            {FName("Whiskers"), false},
            {FName("Bonehead"), false},
            {FName("Lovestruck"), false},
            // Legendary
            {FName("Tomahawk"), false},
            {FName("Rattle"), false},
            {FName("Shredder"), false},
            {FName("Mjolnir"), false},
            // Mythic
            {FName("Money_Bag"), false},
            {FName("Punchin_"), false},
            {FName("Pickle"), false}
        };
    }
    
    FPlayerData(int32 InCurrentCoins, int32 InHighScore, int32 InCurrentScore, TMap<FName, bool>InUnlockStatuses, FName InCurrentPaddleName) : PlayerCoins(InCurrentCoins), PlayerHighScore(InHighScore), PlayersLastScore(InCurrentScore), PaddleUnlockStatuses(InUnlockStatuses), CurrentPaddleName(InCurrentPaddleName) {}
    
    FPlayerData(const FPlayerData& Other)
            : PlayerCoins(Other.PlayerCoins), PlayerHighScore(Other.PlayerHighScore), PlayersLastScore(Other.PlayersLastScore), PaddleUnlockStatuses(Other.PaddleUnlockStatuses), CurrentPaddleName(Other.CurrentPaddleName) {}
    
#pragma region PlayerStats
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
    int32 PlayerCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
    int32 PlayerHighScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
    int32 PlayersLastScore;

	// String : Paddle ID In DT, bool : Unlock Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess))
    TMap<FName, bool> PaddleUnlockStatuses;

#pragma endregion PlayerStats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerItems")
    FName CurrentPaddleName;
// Volume Settings
#pragma region Settings
#pragma endregion Settings
};
