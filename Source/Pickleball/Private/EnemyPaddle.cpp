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
		constexpr float XOuterBounds = 670.f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromCenter = FMath::Clamp(GetActorLocation().X / XOuterBounds, 0,1);

		// Find out later!
		const float ExtraXForceNeeded = ((-16 * ForceMultiplier) * PercentageOfXDistanceFromCenter);
		RandomForce.X = (-32 * ForceMultiplier) + ExtraXForceNeeded;

		// Based on position from center
		constexpr float MinYVal = -8.f;
		constexpr float MaxYVal = 8.f;
		RandomForce.Y = FMath::RandRange(MinYVal * (1 + PercentageOfYDistanceFromCenter), MaxYVal * (1 - PercentageOfYDistanceFromCenter));
		
		const float ExtraZForceNeeded = ((13 / (ForceMultiplier * ForceMultiplier)) * PercentageOfXDistanceFromCenter);
		RandomForce.Z = (26 / (ForceMultiplier * ForceMultiplier)) + ExtraZForceNeeded;

		UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());
		BallInScene->BallMesh->SetWorldLocation(FVector(BallInScene->BallMesh->GetComponentLocation().X, BallInScene->BallMesh->GetComponentLocation().Y, 45));
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



