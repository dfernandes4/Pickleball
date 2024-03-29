// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Paddle.h"

AMainPlayerController::AMainPlayerController()
{
		PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind the touch event
	InputComponent->BindTouch(IE_Pressed, this, &AMainPlayerController::CheckTouchInput);
	InputComponent->BindTouch(IE_Released, this, &AMainPlayerController::CheckTouchInput);
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPaddleMoving)
	{
		MovePaddleSmoothly(MoveStartLocation, MoveTargetLocation, MoveDuration, MoveStartTime);

		// Check if movement should stop
		if (GetWorld()->GetTimeSeconds() - MoveStartTime >= MoveDuration)
		{
			bIsPaddleMoving = false; // Stop moving the paddle
		}
	}
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

void AMainPlayerController::ProcessTouchInput(FVector StartLocation, FVector EndLocation)
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
			PaddleActor->StartSwing(SwipeLength, SwipeDirection, SwipeTime);
		}
	}
	else
	{
		HandleTapInput(InitialTouchLocation);
		
		//if players tap is in the boundary
		
		// Handle tap input
	}
}

void AMainPlayerController::HandleTapInput(FVector TapLocation)
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
					// Start interpolation to smoothly move the paddle to the target location
					MoveStartLocation = PaddleActor->GetActorLocation();
					MoveTargetLocation = HitResult.ImpactPoint + ZOffset;
					MoveDuration = 0.5f; // Adjust the duration of movement as needed
					MoveStartTime = GetWorld()->GetTimeSeconds();
					bIsPaddleMoving = true;
					// Move the paddle to the hit location, if valid
					
				}
			}
		}
	}
}

void AMainPlayerController::MovePaddleSmoothly(const FVector& InMoveStartLocation, const FVector& InMoveTargetLocation,
	  float InMoveDuration,  float InMoveStartTime)
{
	// Calculate how much time has passed since the movement started
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeSinceStarted = CurrentTime - InMoveStartTime;

	// Determine the fraction of the movement that has been completed
	float Alpha = FMath::Clamp(TimeSinceStarted / InMoveDuration, 0.0f, 1.0f);

	// Optionally, use easing for a smoother start/end
	// Alpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);

	// Calculate the current position based on the Alpha value
	FVector CurrentLocation = FMath::Lerp(InMoveStartLocation, InMoveTargetLocation, Alpha);

	// Move the paddle to the new location
	APaddle* PaddleActor = Cast<APaddle>(GetPawn());
	if (PaddleActor != nullptr)
	{
		PaddleActor->SetActorLocation(CurrentLocation);
	}
	
	
}