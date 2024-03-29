// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "EnemyPaddle.generated.h"

UCLASS()
class PICKLEBALL_API AEnemyPaddle : public APaddle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
