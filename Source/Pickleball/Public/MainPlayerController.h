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

	// A method to determine if an input is a swipe and then perform an action
	void ProcessTouchInput(FVector StartLocation, FVector EndLocation) const;

private:
	
	FVector InitialTouchLocation;
	
	bool bIsTouchActive;
	
	const float SwipeThreshold;
	float SwipeStartTime;
	float SwipeEndTime;
};
