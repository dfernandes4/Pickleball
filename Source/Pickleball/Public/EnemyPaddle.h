// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttributes.h"
#include "Paddle.h"
#include "EnemyPaddle.generated.h"

class UPlayScreenWidget;
class UFloatingPawnMovement;
class UNiagaraComponent;

UCLASS()
class PICKLEBALL_API AEnemyPaddle : public APaddle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPaddle();

	UFUNCTION()
	void HitBall();
	
	virtual void FlipPaddle() override;

	UFUNCTION()
	void IncrementForceMultiplier(int NewScore);
	
	UFUNCTION()
	void SetRandomEnemyAttributes();
	
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraComponent* SwingEffect;

	
	FEnemyAttributes* CurrentEnemyAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UDataTable* EnemyAttributes;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	//UStaticMeshComponent* BackgroundPlane;
	
	
private:

	// With current equations max is 5.5x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	float ForceMultiplier;

	
};
