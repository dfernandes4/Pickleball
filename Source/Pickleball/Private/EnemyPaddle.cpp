// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "NiagaraComponent.h"
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

		BallInScene->ApplySwipeForce(FVector(-42,13.5,35), this);
	}
	else
	{
		FVector RandomForce;
		const float BallCurrentZ = BallInScene->BallMesh->GetComponentLocation().Z;
		const float ZRatioToFloor = FMath::Clamp((BallCurrentZ / 40), 0.02, 1);
		
		constexpr float YOuterBounds = 372.f;
		constexpr float XOuterBounds = 670.f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromCenter = GetActorLocation().X / XOuterBounds;

		const float PercentageOfZRatioUsed = (1 - PercentageOfXDistanceFromCenter) * ZRatioToFloor;
		const float XToLoseFromZRatio = (32 * ForceMultiplier) * PercentageOfZRatioUsed;
		
		RandomForce.X = (-32 * ForceMultiplier) + XToLoseFromZRatio;
		
		if(PercentageOfYDistanceFromCenter < 0)
		{
			RandomForce.Y = FMath::RandRange(0.f, -PercentageOfYDistanceFromCenter * 15.f);
		}
		else
		{
			RandomForce.Y = FMath::RandRange(PercentageOfYDistanceFromCenter * -15.f, 0.f);
		}
		
		RandomForce.Z = FMath::Clamp(((26 / ForceMultiplier) * PercentageOfXDistanceFromCenter) + (.3 * (((26 / ForceMultiplier) * PercentageOfXDistanceFromCenter) * (1 - ZRatioToFloor))), 0.f, (26 / ForceMultiplier));

		UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());
		BallInScene->ApplySwipeForce(RandomForce, this);
	}
	
	
}



