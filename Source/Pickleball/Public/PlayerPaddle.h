// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "PlayerPaddle.generated.h"
class UNiagaraComponent;
class AMainGamemode;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PICKLEBALL_API APlayerPaddle : public APaddle
{
	GENERATED_BODY()

public:
	
	// Sets default values for this pawn's properties
	APlayerPaddle();

	UFUNCTION()
	void StartSwing(const FVector& BallCurrentLocation);

	UFUNCTION()
	void AddPlayerCoins(int32 CoinsAmount);
	
	UFUNCTION()
	float GetScore() const;

	virtual void BeginPlay() override;
	
	virtual void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void SetIsPlayersTurn(bool bIsPlayersTurnIn);
	
	virtual void FlipPaddle() override;

#pragma region Save/Load

	UFUNCTION()
	void SaveAllStats();

	UFUNCTION()
	void SavePlayerCoins(int32 CoinAmount);

	UFUNCTION()
	void SavePlayerHighScore(int32 CoinAmount);

	UFUNCTION()
	void SavePlayerPaddleUnlockStatuses(const TMap<FString, bool>& PaddleUnlockStatusesIn);

	UFUNCTION()
	void LoadGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraComponent* SwingEffect;
	
#pragma endregion Save/Load
	
private:

#pragma region Swiping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FVector CurrentSwipeDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	float CurrentSwipeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsSwingActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FTimerHandle SwingTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	float SwipeForceMultiplier;
	

	

#pragma endregion Swiping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 CurrentCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 HighScore;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 CurrentScore;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	TMap<FString, bool> PaddleUnlockStatuses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassVariables, meta = (AllowPrivateAccess))
	AMainGamemode* MainGamemode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassVariables, meta = (AllowPrivateAccess))
	bool bIsPlayersTurn;
	
};
