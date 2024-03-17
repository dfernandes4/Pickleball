// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Paddle.h"

AMainPlayerController::AMainPlayerController()
{
	bIsTouchActive = false;
	float SwipeStartTime = 0.0f;
	float SwipeEndTime = 0.0f;
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind the touch event
	InputComponent->BindTouch(IE_Pressed, this, &AMainPlayerController::CheckTouchInput);
	InputComponent->BindTouch(IE_Released, this, &AMainPlayerController::CheckTouchInput);
}

void AMainPlayerController::CheckTouchInput(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1) // Assuming single-touch for simplicity
	{
		if (!bIsTouchActive)
		{
			// On touch start
			SwipeStartTime = GetWorld()->GetTimeSeconds();
			InitialTouchLocation = Location;
			bIsTouchActive = true;
		}
		else
		{
			// On touch end
			SwipeEndTime = GetWorld()->GetTimeSeconds();
			ProcessTouchInput(InitialTouchLocation, Location);
			bIsTouchActive = false;
		}
	}
}

void AMainPlayerController::ProcessTouchInput(FVector StartLocation, FVector EndLocation) const
{
	FVector SwipeDirection = EndLocation - StartLocation;
	float SwipeLength = SwipeDirection.Size();
	

	// Determine if the swipe is long enough to be considered a swipe and not a tap
	if (SwipeLength > SwipeThreshold) 
	{
		const float SwipeTime = SwipeEndTime - SwipeStartTime;
		APaddle* PaddleActor = Cast<APaddle>(GetPawn()); 
		if (PaddleActor)
		{
			PaddleActor->Swing(SwipeLength, SwipeDirection, SwipeTime);
		}
	}
	else
	{
		// Handle tap input
	}
}
