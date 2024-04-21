// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"

#include "Ball.h"
#include "Engine.h"
#include "MainGamemode.h"
#include "MainPlayerController.h"
#include "PaperSpriteComponent.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bIsFirstSwing = true;

	CurrentScore = 0;
	
}

void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());
}

void APlayerPaddle::StartSwing()
{
	// If swipe is going up
	if(!bIsSwingActive)
	{
		bIsSwingActive = true;

		//Flip paddle after swipe
		FRotator CurrentRotation = PaddleSprite->GetRelativeRotation();
		if(!bIsFacingLeft)
		{
			bIsFacingLeft = true;
			CurrentRotation.Yaw += 90.0f;
		}
		else
		{
			bIsFacingLeft = false;
			CurrentRotation.Yaw -= 90.0f;
		}
		
		this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);

		GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &APlayerPaddle::FinishSwing, .5f, false);
		
		//play the sound effect
		//play particle effect
		
		if(bIsInHittingZone)
		{
			if (IsValid(BallInScene))
			{
				FVector ScaledPaddleVelocity = (Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()) * .01;

				UE_LOG(LogTemp, Warning, TEXT("Paddle Velocity: %s"), *(Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()).ToString());
				
				//Range of X-Y-Z Should be between 10-40
				const float ForceXDistance = FMath::Clamp(ScaledPaddleVelocity.X, 30.0f, 50.0f);
				const float ForceYDistance = FMath::Clamp(ScaledPaddleVelocity.Y, -75.0f, 75.0f);
				const float ForceZDistance = 25;
				
				// Normalize the swipe direction to use as a direction vector in the world
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				
				// Apply the force in the calculated direction with the calculated magnitude
				if(bIsFirstSwing)
				{
					BallInScene->BallMesh->SetEnableGravity(true);
					bIsFirstSwing = false;
				}
				
				CurrentScore++;
				
				if(MainGamemode)
				{
					MainGamemode->OnScoreUpdated.Broadcast(CurrentScore);
				}
				
				BallInScene->ApplySwipeForce(Force, this);
				
			}
		}
		
	}
}

void APlayerPaddle::FinishSwing()
{
	bIsSwingActive = false;
}

float APlayerPaddle::GetScore() const
{
	return CurrentScore;
}

void APlayerPaddle::OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPaddleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor->IsA(ABall::StaticClass()))
	{
		StartSwing();
	}
}

