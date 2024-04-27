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
}

void AEnemyPaddle::HitBall()
{
	//Might change values, ENEMY CANNOT MISS!
	if(bIsFirstSwing)
	{
		BallInScene->BallMesh->SetEnableGravity(true);
		bIsFirstSwing = false;
	}
	
	FVector RandomForce;
	RandomForce.X = -32;
	constexpr float YOuterBounds = 372.f;
	const float PercentageOfDistanceFromCenter = GetActorLocation().Z / YOuterBounds;
	if(PercentageOfDistanceFromCenter < 0)
	{
		RandomForce.Y = FMath::RandRange(0.f, -PercentageOfDistanceFromCenter * 45.f);
	}
	else
	{
		RandomForce.Y = FMath::RandRange(PercentageOfDistanceFromCenter * -45.f, 0.f);
	}
	RandomForce.Z = 30;
	
	BallInScene->ApplySwipeForce(RandomForce, this);
}



