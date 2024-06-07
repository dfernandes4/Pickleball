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
	UFUNCTION()
	void StopHitting();
	// Sets default values for this pawn's properties
	AEnemyPaddle();

	UFUNCTION()
	void HitBall();
	
	virtual void FlipPaddle() override;

	UFUNCTION()
	void IncrementForceMultiplier(int NewScore);
	
	UFUNCTION()
	void SetRandomEnemyAttributes();

	UFUNCTION()
	void SetIsEnemiesTurn(bool bIsTurn);

	UFUNCTION()
	void SetCurrentRow(int32 Row);

	UFUNCTION()
	int32 GetCurrentRow() const;

	virtual void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void AdjustEnemySpeed(const FVector& BallVelocity, const FVector& HittingLocation);

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

	UPROPERTY()
	bool bIsEnemiesTurn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess))
	USoundBase* PaddleSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	int32 CurrentRow;
	
};
