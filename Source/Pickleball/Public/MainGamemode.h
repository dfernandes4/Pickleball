// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGamemode.generated.h"

class APlayerPaddle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int, NewScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverDelegate);

UCLASS()
class PICKLEBALL_API AMainGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGamemode();
	
	FOnGameOverDelegate OnGameOver;

	void GameOver();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MasterSoundClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	APlayerPaddle* PlayerPaddle;


	FOnScoreUpdated OnScoreUpdated;
};
