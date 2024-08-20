// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGamemode.generated.h"

class APlayerPaddle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int, NewScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPaddleSelectedDelegate, UPaddleToCollectWidget*, PaddleToCollectWidgetSelected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPaddleBoughtDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinAmountChangedDelegate, int32 , NewCoinAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownKickoffFinishedDelegate);

UCLASS()
class PICKLEBALL_API AMainGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void GameOver();

	UFUNCTION()
	void OnGameStart();
	
	AMainGamemode();
	
	FOnGameOverDelegate OnGameOver;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameLoaded();
	
	UFUNCTION()
	void OnLoadingScreenFinished();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USceneComponent* Scene;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* BattleMusic;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* HomeScreenMusic;
	
	FOnScoreUpdated OnScoreUpdated;

	FOnNewPaddleSelectedDelegate OnPaddleSelected;

	FPaddleBoughtDelegate OnPaddleBought;
	
	FOnCoinAmountChangedDelegate OnCoinAmountChanged;

	FOnCountdownKickoffFinishedDelegate  OnCountdownKickoffFinished;

	bool bIsGameActive;

	UPROPERTY()
	class AEnemyAIController* CachedEnemyAIController;

	UPROPERTY()
	class UPickleBallGameInstance* PickleBallGameInstance;
};
