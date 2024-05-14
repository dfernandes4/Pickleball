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

	SwingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	SwingEffect->SetupAttachment(SceneComponent);
	

	ForceMultiplier = 1;
	XForceToTest = 0;
}

void AEnemyPaddle::HitBall()
{
	FlipPaddle();
	
	//Might change values, ENEMY CANNOT MISS!
	if(bIsFirstSwing)
	{
		BallInScene->BallMesh->SetEnableGravity(true);
		bIsFirstSwing = false;

		FVector RandomForceToTest;
		RandomForceToTest.X = XForceToTest;
		RandomForceToTest.Y = 0;
		RandomForceToTest.Z = ZForceMultiplierFORTESTING;

		//BallInScene->ApplySwipeForce(FVector(-42,13.5,35), this);

		//Tester
		BallInScene->ApplySwipeForce(FVector(RandomForceToTest),  this);
		UE_LOG(LogTemp, Warning, TEXT("ForceBeingTested: %s"), *RandomForceToTest.ToString());
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
		
		const float MinZ = (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(RandomForce.X, 3.f))) +
							(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(RandomForce.X, 2.f))) + (1.092 * RandomForce.X) + 54.61;

		// 7.065×10−13x7+5.755×10−10x6+1.978×10−7x5+3.731×10−5x4+4.212×10−3x3+0.290x2+11.91x+262.7375
		const float MaxZ = (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(RandomForce.X, 7.f))) + 
						(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(RandomForce.X, 6.f))) + 
						(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(RandomForce.X, 5.f))) + 
						(.0000373143f * FMath::Pow(RandomForce.X, 4.f)) + 
						(.00421209 * FMath::Pow(RandomForce.X, 3.f)) + 
						(0.290215 * FMath::Pow(RandomForce.X, 2.f)) + (11.9063 * RandomForce.X) + 262.738;

		RandomForce.Z = MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation);
		/*
			const float ExtraZForceNeeded = ((12.7 * ZForceMultiplierFORTESTING) * PercentageOfXDistanceFromCenter);
			RandomForce.Z = ((26.3 * ZForceMultiplierFORTESTING) + ExtraZForceNeeded);
		*/

		UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		BallInScene->BallMesh->SetWorldLocation(FVector(BallInScene->BallMesh->GetComponentLocation().X, BallInScene->BallMesh->GetComponentLocation().Y, 45));
		UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());
		BallInScene->ApplySwipeForce(RandomForce, this);
	}
	
	
}

void AEnemyPaddle::FlipPaddle()
{
	//Flip paddle after swipe
	FRotator SwingEffectCurrentRotation = SwingEffect->GetRelativeRotation();
	FVector  SwingEffectCurrentLocation = SwingEffect->GetRelativeLocation();
	FRotator CurrentRotation = PaddleSprite->GetRelativeRotation();
	if(!bIsFacingLeft)
	{
		bIsFacingLeft = true;
		CurrentRotation.Yaw -= 90.0f;
		SwingEffectCurrentRotation.Roll +=180.0f;
		SwingEffectCurrentLocation.X -= 22.0;
		SwingEffectCurrentLocation.Y -= 40.0;
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw += 90.0f;
		SwingEffectCurrentRotation.Roll -=180.0f;
		SwingEffectCurrentLocation.X += 22.0;
		SwingEffectCurrentLocation.Y += 40.0;
	}
	SwingEffect->ResetSystem();
		
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
	this->SwingEffect->SetRelativeRotation(SwingEffectCurrentRotation,false,nullptr,ETeleportType::TeleportPhysics);

	this->SwingEffect->SetRelativeLocation(SwingEffectCurrentLocation,false,nullptr,ETeleportType::TeleportPhysics);

}



