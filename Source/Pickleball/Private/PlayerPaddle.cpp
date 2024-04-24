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

	bIsPlayersTurn = true;
	
}

void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());
}

void APlayerPaddle::StartSwing(const FVector& BallCurrentLocation)
{
	// If swipe is going up
	if(bIsPlayersTurn)
	{
		bIsPlayersTurn = false;

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
		
		//play the sound effect
		//play particle effect
		
		if(bIsInHittingZone)
		{
			if (IsValid(BallInScene))
			{
				FVector ScaledPaddleVelocity = (Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()) * .01;

				UE_LOG(LogTemp, Warning, TEXT("Paddle Velocity: %s"), *(Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()).ToString());
				
				//Range of X-Y-Z Should be between 10-40
				float MinXOffset = 25 * (BallCurrentLocation.X / -670);
				float MaxXOffset = 10 * (BallCurrentLocation.X / -670);;
				const float ForceXDistance = FMath::Clamp(ScaledPaddleVelocity.X, 15.0f + MinXOffset, 30.0f + MaxXOffset);
				const float ForceYDistance = FMath::Clamp(ScaledPaddleVelocity.Y, -10.0f, 10.0f);
				float ForceZDistance = 20;
				if(BallCurrentLocation.Z < 185)
				{
					// 10 is the addition needed to make the ball go over the fence if it is on the floor)
					ForceZDistance += (15 * (BallCurrentLocation.Z / 185));
					if(BallCurrentLocation.X < -470 || ForceYDistance > 2 || ForceYDistance < -2)
					{
						ForceZDistance += 2;
					}
					{
						ForceZDistance += 5;
					}
				}
				
				// Normalize the swipe direction to use as a direction vector in the world
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				UE_LOG(LogTemp, Warning, TEXT("Force: %s"), *Force.ToString());
				
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
		StartSwing(Cast<ABall>(OtherActor)->BallMesh->GetComponentLocation());
	}
}

void APlayerPaddle::SetIsPlayersTurn(bool bIsPlayersTurnIn)
{
	bIsPlayersTurn = bIsPlayersTurnIn;
}


