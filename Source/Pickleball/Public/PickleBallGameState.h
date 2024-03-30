// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PickleBallGameState.generated.h"

class AEnemyPaddle;
class APlayerPaddle;
/**
 * 
 */
UCLASS()
class PICKLEBALL_API APickleBallGameState : public AGameState
{
	GENERATED_BODY()

	APickleBallGameState();

	
public:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	APlayerPaddle* PlayerPaddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	AEnemyPaddle* EnemyPaddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	TSubclassOf<AEnemyPaddle> EnemyPaddleClass;
};
