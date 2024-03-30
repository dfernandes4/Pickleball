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
	
	virtual void Tick(float DeltaTime) override;	
protected:
	virtual void SetupInputComponent() override;

	void CheckTouchInput(ETouchIndex::Type FingerIndex, FVector Location);

	void MovePaddleSmoothly(const FVector& InMoveStartLocation, const FVector& InMoveTargetLocation,float InMoveDuration, float InMoveStartTime);

	void MoveToZone(const FVector& ZoneTargetLocation);
	
	void HandleTapInput(FVector TapLocation);

	// A method to determine if an input is a swipe and then perform an action
	void ProcessTouchInput(const FVector& StartLocation,const FVector& EndLocation);

private:
	bool bIsPaddleMoving = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	FVector MoveStartLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	FVector MoveTargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	float MoveDuration; // Or any other suitable duration
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	float MoveStartTime;
	
	FVector InitialTouchLocation;
	
	bool bIsTouchActive = false;
	const float SwipeThreshold = 75.0f;
	float SwipeStartTime = 0.0f;
	float SwipeEndTime = 0.0f;
	float TraceDistance = 10000.0f;
	const FVector ZOffset = FVector(0,0,90);
};
