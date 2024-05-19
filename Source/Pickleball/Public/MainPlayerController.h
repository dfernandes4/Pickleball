// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "MainPlayerController.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPurchaseCompletedDelegate, int32, CoinsAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstTouchDelegate);

UCLASS()
class PICKLEBALL_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();
	
	virtual void SetupInputComponent() override;
	
	UFUNCTION()
	FVector GetPaddleVelocity() const;

	UFUNCTION()
	void InitiatePurchaseRequest(const FString& ProductId);
	void HandlePurchaseCompletion(bool bWasSuccessful, const TArray<FUniqueOfferId>& Offers, const FString& ErrorMsg);

	UFUNCTION()
	void OnGameOver();
	
	FOnPurchaseCompletedDelegate  OnPurchaseCompleted;
	FOnFirstTouchDelegate OnFirstTouch;
	

protected:
	
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchMoved(const ETouchIndex::Type FingerIndex, const FVector Location);

private:
	bool bIsTouching = false;
	FVector2D InitialTouchLocation;
	FVector2D TouchLocation;
	FVector TouchOffset;

	UPROPERTY()
	FVector PaddleStartLocation;
	UPROPERTY()
	FVector NewPaddleLocation;
	UPROPERTY()
	float CurrentDeltaTime;
	UPROPERTY()
	FVector PaddleVelocity;
	UPROPERTY()
	bool bIsFirstTouch;;

};
