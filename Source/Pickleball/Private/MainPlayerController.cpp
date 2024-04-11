// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "PlayerPaddle.h"

AMainPlayerController::AMainPlayerController()
{
		PrimaryActorTick.bCanEverTick = true;
		MoveStartTime = 0.0f;

	//TODO: turn this off before packaging
		SetShowMouseCursor(true);
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
		MovePaddleSmoothly(MoveStartLocation, MoveTargetLocation, MoveStartTime);

		// Check if movement should stop
		if (GetWorld()->GetTimeSeconds() - MoveStartTime >= MOVE_DURATION)
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

void AMainPlayerController::ProcessTouchInput(const FVector& StartLocation,const FVector& EndLocation)
{
	const float ScreenYDistance = EndLocation.Y - StartLocation.Y;
	const float ScreenXDistance = EndLocation.X - StartLocation.X;

	APlayerPaddle* PlayerPaddleActor = Cast<APlayerPaddle>(GetPawn());
	
	// Determine if the swipe is long enough to be considered a swipe and not a tap
	if (FMath::Abs(ScreenXDistance) > SWIPE_THRESHOLD || FMath::Abs(ScreenYDistance) > SWIPE_THRESHOLD) 
	{
		const float SwipeTime = SwipeEndTime - SwipeStartTime;
		if (PlayerPaddleActor)
		{
			PlayerPaddleActor->StartSwing(ScreenYDistance, ScreenXDistance, SwipeTime);
		}
	}
	else
	{
		if(!PlayerPaddleActor->GetIsFirstSwing())
		{
			HandleTapInput(InitialTouchLocation);
		}
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
		APlayerPaddle* PlayerPaddleActor = Cast<APlayerPaddle>(GetPawn());
		if (PlayerPaddleActor != nullptr)
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
					MoveStartLocation = PlayerPaddleActor->GetActorLocation();
					MoveTargetLocation = HitResult.ImpactPoint;
					MoveTargetLocation += ZOffset;
					MoveStartTime = GetWorld()->GetTimeSeconds();
					bIsPaddleMoving = true;
					// Move the paddle to the hit location, if valid
					
				}
			}
		}
	}
}
void AMainPlayerController::MoveToZone(const FVector& ZoneTargetLocation)
{
    // Setup automatic movement towards ZoneTargetLocation
    // This could be initiated based on game logic or events
    MoveStartLocation = GetPawn()->GetActorLocation();
    MoveTargetLocation = ZoneTargetLocation;
    MoveStartTime = GetWorld()->GetTimeSeconds();
    bIsPaddleMoving = true;
	
}

void AMainPlayerController::MovePaddleSmoothly(const FVector& InMoveStartLocation, const FVector& InMoveTargetLocation,
	float InMoveStartTime)
{
	// Calculate how much time has passed since the movement started
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeSinceStarted = CurrentTime - InMoveStartTime;

	// Determine the fraction of the movement that has been completed
	float Alpha = FMath::Clamp(TimeSinceStarted / MOVE_DURATION, 0.0f, 1.0f);

	// Optionally, use easing for a smoother start/end
	// Alpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);

	// Calculate the current position based on the Alpha value
	FVector CurrentLocation = FMath::Lerp(InMoveStartLocation, InMoveTargetLocation, Alpha);

	// Move the paddle to the new location
	APlayerPaddle* PlayerPaddleActor = Cast<APlayerPaddle>(GetPawn());
	if (PlayerPaddleActor != nullptr)
	{
		PlayerPaddleActor->SetActorLocation(CurrentLocation);
	}
	
}