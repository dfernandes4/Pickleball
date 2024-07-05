// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "PaddleToCollectWidget.h"
#include "PlayerData.h"
#include "PlayerPaddle.generated.h"
class ISaveGameInterface;
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
	float GetScore() const;

	UFUNCTION()
	void OnGameOver();

	UFUNCTION()
	bool OnPaddleBought(FName PaddleName);

	UFUNCTION()
	void OnPaddleSelected(FName PaddleName);
	
	UFUNCTION()
	void SetIsPlayersTurn(bool bIsPlayersTurnIn);
    
    UFUNCTION()
    void OnGameLoaded();
	
	virtual void FlipPaddle() override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

#pragma region Save/Load

	UFUNCTION()
	FPlayerData GetCurrentPlayerData();

	UFUNCTION()
	int32 GetHighScore() const;

	UFUNCTION()
	int32 GetPlayerCoins() const;

	UFUNCTION()
	int32 GetCoinsEarnedFromLastMatch() const;

	UFUNCTION()
	TMap<FName, bool> GetPaddleUnlockStatuses () const;

	UFUNCTION()
	void AddCoins(int32 CoinsToAdd);

	UFUNCTION()
	int32 GetCurrentScore() const;

	UFUNCTION()
	void SetCurrentScore(int32 ScoreToSet);
	UFUNCTION()
	void SetCurrentPaddle(FName CurrrentPaddleNameIn);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess))
	UNiagaraComponent* SwingEffect;

#pragma endregion Swiping

#pragma region PlayerStats
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 CurrentCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 CoinsEarnedFromLastMatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 HighScore;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	int32 CurrentScore;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
    int32 LastScore;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	TMap<FName, bool> PaddleUnlockStatuses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess))
	UDataTable* PaddleDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess))
	int32 CoinMultiplier;

#pragma endregion PlayerStats
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassVariables, meta = (AllowPrivateAccess))
	AMainGamemode* MainGamemode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ClassVariables, meta = (AllowPrivateAccess))
	bool bIsPlayersTurn;
	
	ISaveGameInterface* SaveGameInterface;

	UPROPERTY()
	FName CurrrentPaddleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess))
	USoundBase* PaddleSoundEffect;
};
