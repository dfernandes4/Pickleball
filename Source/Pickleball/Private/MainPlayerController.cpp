// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Paddle.h"

AMainPlayerController::AMainPlayerController()
{
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
		HandleTapInput(InitialTouchLocation);
		
		//if players tap is in the boundary
		
		// Handle tap input
	}
}

	void AMainPlayerController::HandleTapInput(FVector TapLocation) const
	{
		// Convert TapLocation from screen to world location
		FVector WorldLocation, WorldDirection;
		if (DeprojectScreenPositionToWorld(TapLocation.X, TapLocation.Y, WorldLocation, WorldDirection))
		{
			APaddle* PaddleActor = Cast<APaddle>(GetPawn());
			if (PaddleActor != nullptr)
			{
				// Perform a line trace to find a suitable target location on the court
				FHitResult HitResult;
				FCollisionQueryParams TraceParams(FName(TEXT("CourtTrace")), true, GetPawn());
				if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * TraceDistance, ECC_Visibility, TraceParams))
				{
					// Check if the hit actor has the court tag
					if (HitResult.GetActor()->ActorHasTag(TEXT("Court")))
					{
						// Move the paddle to the hit location, if valid
						PaddleActor->SetActorLocation(HitResult.ImpactPoint);
					}
				}
			}
		}
}
