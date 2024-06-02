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

UCLASS()
class PICKLEBALL_API AMainGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void GameOver();
	
	AMainGamemode();
	
	FOnGameOverDelegate OnGameOver;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MasterSoundClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MusicSoundClass;
	
	FOnScoreUpdated OnScoreUpdated;

	FOnNewPaddleSelectedDelegate OnPaddleSelected;

	FPaddleBoughtDelegate OnPaddleBought;
	
	FOnCoinAmountChangedDelegate OnCoinAmountChanged;
};
