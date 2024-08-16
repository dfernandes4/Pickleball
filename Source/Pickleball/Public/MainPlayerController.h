// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Interfaces/OnlinePurchaseInterface.h"

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


    // Function to initiate a purchase request
    UFUNCTION(BlueprintCallable, Category = "In-App Purchases")
    void InitiatePurchaseRequest(const FString& ProductId);

	UFUNCTION()
	void LoginToGameCenter();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	UFUNCTION()
	void ShowLeaderboard(FName CategoryName);
	UFUNCTION()
	bool IsLoggedInToGameCenter();
    UFUNCTION()
    void SubmitHighscore(int32 Score, FName CategoryName);
    
	UFUNCTION()
	void OnGameOver();

	FOnPurchaseCompletedDelegate  OnPurchaseCompleted;
	FOnFirstTouchDelegate OnFirstTouch;
	FDelegateHandle DelegateHandle;

protected:
	
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchMoved(const ETouchIndex::Type FingerIndex, const FVector Location);

private:


    // Callback for when the offer query is complete
    void OnQueryOffersComplete(bool bWasSuccessful, const TArray<FUniqueOfferId>& Offers, const FString& ErrorMsg);
	
    // Function to initiate the purchase of an offer
    void PurchaseOffer(FOnlineStoreOfferRef Offer);

    // Callback for when the purchase is complete
    void HandlePurchaseCompletion(const FOnlineError& Result, const TSharedRef<FPurchaseReceipt>& PurchaseReceipt);

	
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
