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

	SwingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	SwingEffect->SetupAttachment(SceneComponent);
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

		//Tester
		//BallInScene->ApplySwipeForce(FVector(-128,0,-2.7),  this);
	}
	else
	{
		FVector RandomForce;
		
		constexpr float YOuterBounds = 372.f;
		constexpr float XOuterBounds = 670.f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromCenter = FMath::Clamp(GetActorLocation().X / XOuterBounds, 0,1);
		
		const float ExtraXForceNeeded = ((-15 / ForceMultiplier) * PercentageOfXDistanceFromCenter);	
		RandomForce.X = (-33 * ForceMultiplier) + ExtraXForceNeeded;

		// Based on position from center
		constexpr float MinYVal = -8.f;
		constexpr float MaxYVal = 8.f;
		RandomForce.Y = ZForceMultiplierFORTESTING * FMath::RandRange(MinYVal * (1 + PercentageOfYDistanceFromCenter), MaxYVal * (1 - PercentageOfYDistanceFromCenter));

		// (0.00324 × X^2) + (0.820 × X) + 49.23
		RandomForce.Z = (0.00324 * FMath::Pow(RandomForce.X, 2)) + (0.820 * RandomForce.X) + 49.23;

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



