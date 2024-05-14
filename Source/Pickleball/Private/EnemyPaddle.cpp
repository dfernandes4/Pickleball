// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "NiagaraComponent.h"
#include "Ball.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AEnemyPaddle::AEnemyPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 400.f;

	ForceMultiplier = 1;
}

void AEnemyPaddle::HitBall()
{
	FlipPaddle();
	
	//Might change values, ENEMY CANNOT MISS!
	if(bIsFirstSwing)
	{
		BallInScene->BallMesh->SetEnableGravity(true);
		bIsFirstSwing = false;

		BallInScene->ApplySwipeForce(FVector(-42,13.5,35), this);
	}
	else
	{
		FVector RandomForce;
		
		constexpr float YOuterBounds = 372.f;
		constexpr float FarthestHittingLocation = 895.0f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromFarthestHittingLocation = FMath::Clamp(GetActorLocation().X / FarthestHittingLocation, 0,1);
		
		RandomForce.X = (-32 * ForceMultiplier);

		// Based on position from center
		constexpr float MinYVal = -8.f;
		constexpr float MaxYVal = 8.f;
		RandomForce.Y = FMath::RandRange(MinYVal * (1 + PercentageOfYDistanceFromCenter), MaxYVal * (1 - PercentageOfYDistanceFromCenter));

		// Interpolation for Z
		const float MinZ = (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(RandomForce.X, 3.f))) +
							(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(RandomForce.X, 2.f))) + (1.092 * RandomForce.X) + 54.61;
		
		const float MaxZ = (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(RandomForce.X, 7.f))) + 
						(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(RandomForce.X, 6.f))) + 
						(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(RandomForce.X, 5.f))) + 
						(.0000373143f * FMath::Pow(RandomForce.X, 4.f)) + 
						(.00421209 * FMath::Pow(RandomForce.X, 3.f)) + 
						(0.290215 * FMath::Pow(RandomForce.X, 2.f)) + (11.9063 * RandomForce.X) + 262.738;

		RandomForce.Z = MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation);

		// Set ball to perfect height
		UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		BallInScene->BallMesh->SetWorldLocation(FVector(BallInScene->BallMesh->GetComponentLocation().X, BallInScene->BallMesh->GetComponentLocation().Y, 45));

		// Apply force
		UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());
		BallInScene->ApplySwipeForce(RandomForce, this);
	}
	
	
}

void AEnemyPaddle::FlipPaddle()
{
	//Flip paddle after swipe
	FRotator CurrentRotation = PaddleSprite->GetRelativeRotation();
	if(!bIsFacingLeft)
	{
		bIsFacingLeft = true;
		CurrentRotation.Yaw -= 90.0f;
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw += 90.0f;
	}
		
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
}



