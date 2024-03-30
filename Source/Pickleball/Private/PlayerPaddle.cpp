// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"

#include "Ball.h"
#include "Engine.h"
#include "PaperSpriteComponent.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerPaddle::OnPaddleBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerPaddle::OnPaddleEndOverlap);
	
	bIsInHittingZone = false;
	BallInScene = nullptr;

	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(PaddleSprite);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment((CameraComponent));
}

void APlayerPaddle::StartSwing(float ScreenYDistance, float ScreenXDistance, float SwipeTime)
{
	
	// If swipe is going up
	if((ScreenYDistance < 0) && !bIsSwingActive)
	{
		bIsSwingActive = true;

		//Flip paddle after swipe
		FRotator NewRotation;
		if(!bIsFacingLeft)
		{
			bIsFacingLeft = true;
			NewRotation = FRotator(0.0f, GetActorRotation().Yaw + 180.0f, 0.0f);
		}
		else
		{
			bIsFacingLeft = false;
			NewRotation = FRotator(0.0f, GetActorRotation().Yaw - 180.0f, 0.0f);
		}
		
		this->SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);

		GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &APlayerPaddle::FinishSwing, .5f, false);
		
		//play the sound effect
		//play particle effect
		
		if(bIsInHittingZone)
		{
			if (IsValid(BallInScene))
			{
				constexpr float RelativeAdjustmentToWorld = 0.075;
				
				//Range of X-Y-Z Should be between 10-40
				const float ForceXDistance = FMath::Clamp(((-ScreenYDistance * RelativeAdjustmentToWorld) / (SwipeTime * 2.5)), 20.0f, 50.0f);
				const float ForceYDistance = FMath::Clamp(ScreenXDistance * RelativeAdjustmentToWorld, -75.0f, 75.0f);
				const float ForceZDistance = FMath::Clamp(-ScreenYDistance * RelativeAdjustmentToWorld, 30.0f, 75.0f);
				
				// Normalize the swipe direction to use as a direction vector in the world
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				
				// Apply the force in the calculated direction with the calculated magnitude
				BallInScene->ApplySwipeForce(Force);
				
			}
		}
		
	}
}

void APlayerPaddle::FinishSwing()
{
	bIsSwingActive = false;
}