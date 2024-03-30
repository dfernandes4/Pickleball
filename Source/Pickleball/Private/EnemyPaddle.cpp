// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "Ball.h"


// Sets default values
AEnemyPaddle::AEnemyPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyPaddle::HitBall() const
{
	//Might change values, ENEMY CANNOT MISS!
	FVector RandomForce;
	RandomForce.X = FMath::RandRange(35.f, 40.f);
	RandomForce.Y = FMath::RandRange(-50.f, 50.f);
	RandomForce.Z = FMath::RandRange(30.f, 40.f);
	
	BallInScene->ApplySwipeForce(RandomForce);
}


