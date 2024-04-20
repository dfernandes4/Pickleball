// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "PlayerPaddle.generated.h"

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
	void StartSwing(float ScreenYDistance, float ScreenXDistance, float SwipeTime);
	UFUNCTION()
	void FinishSwing();
	
	UFUNCTION()
	float GetScore() const;

	virtual void BeginPlay() override;
	
	virtual void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:

#pragma region Swiping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FVector CurrentSwipeDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	float CurrentSwipeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsSwingActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsFacingLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FTimerHandle SwingTimerHandle;

	
	

#pragma endregion Swiping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	float CurrentCoinCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	float CurrentScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	AMainGamemode* MainGamemode;
};
