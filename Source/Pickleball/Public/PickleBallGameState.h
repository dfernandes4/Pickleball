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
class PICKLEBALL_API APickleBallGameState : public AGameStateBase
{
	GENERATED_BODY()

	APickleBallGameState();

	
public:

	virtual void BeginPlay() override;
};
