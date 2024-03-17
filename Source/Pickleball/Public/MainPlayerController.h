// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();
protected:
	virtual void SetupInputComponent() override;

	void CheckTouchInput(ETouchIndex::Type FingerIndex, FVector Location);

	
	void HandleTapInput(FVector TapLocation) const;

	// A method to determine if an input is a swipe and then perform an action
	void ProcessTouchInput(FVector StartLocation, FVector EndLocation) const;

private:
	
	FVector InitialTouchLocation;
	
	bool bIsTouchActive = false;
	const float SwipeThreshold = 75.0f;
	float SwipeStartTime = 0.0f;
	float SwipeEndTime = 0.0f;
	float TraceDistance = 1000.0f;
};
