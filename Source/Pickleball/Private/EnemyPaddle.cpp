// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "Ball.h"
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
	//Might change values, ENEMY CANNOT MISS!
	if(bIsFirstSwing)
	{
		BallInScene->BallMesh->SetEnableGravity(true);
		bIsFirstSwing = false;

		BallInScene->ApplySwipeForce(FVector(-32,10,30), this);
	}
	else
	{
		FVector RandomForce;
		RandomForce.X = -32 * ForceMultiplier;
		constexpr float YOuterBounds = 372.f;
		constexpr float XOuterBounds = 670.f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromCenter = GetActorLocation().X / XOuterBounds;
		
		if(PercentageOfYDistanceFromCenter < 0)
		{
			RandomForce.Y = FMath::RandRange(0.f, -PercentageOfYDistanceFromCenter * 15.f);
		}
		else
		{
			RandomForce.Y = FMath::RandRange(PercentageOfYDistanceFromCenter * -15.f, 0.f);
		}
		RandomForce.Z = (26 / ForceMultiplier) * PercentageOfXDistanceFromCenter; // + ((FMath::Abs(RandomForce.Y/15) * 15));
		const float BallCurrentZ = BallInScene->BallMesh->GetComponentLocation().Z;
		if(BallCurrentZ < 20)
		{
			RandomForce.Z += (1 - (BallCurrentZ / 20)) *  10;
		}

		UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());
		BallInScene->ApplySwipeForce(RandomForce, this);
	}
	
	
}



