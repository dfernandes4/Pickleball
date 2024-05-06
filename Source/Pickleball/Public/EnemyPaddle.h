// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "EnemyPaddle.generated.h"

class UFloatingPawnMovement;
class UNiagaraComponent;

UCLASS()
class PICKLEBALL_API AEnemyPaddle : public APaddle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPaddle();

	void HitBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects, meta = (AllowPrivateAccess))
	UNiagaraComponent* SwingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects, meta = (AllowPrivateAccess))
	UNiagaraComponent* HitEffect;
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	float ForceMultiplier;

	
};
