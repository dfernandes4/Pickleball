// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "PlayerPaddle.generated.h"
class UPickleballSaveGame;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void SetIsPlayersTurn(bool bIsPlayersTurnIn);
	
	virtual void FlipPaddle() override;

#pragma region Save/Load

	UFUNCTION()
	void SaveAllStats();

	UFUNCTION()
	void SavePlayerCoins();

	UFUNCTION()
	void SavePlayerHighScore();

	UFUNCTION()
	void SavePlayerPaddleUnlockStatuses();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	int32 GetHighScore() const;

	UFUNCTION()
	int32 GetPlayerCoins() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraComponent* SwingEffect;
	
#pragma endregion Save/Load
	
private:

	UFUNCTION()
	UPickleballSaveGame* GetOrCreateSaveGame();

#pragma region Swiping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FVector CurrentSwipeDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	float CurrentSwipeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsSwingActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FTimerHandle SwingTimerHandle;

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
