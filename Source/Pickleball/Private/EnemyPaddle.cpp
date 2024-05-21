// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "NiagaraComponent.h"
#include "Ball.h"
#include "MainGamemode.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AEnemyPaddle::AEnemyPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	//Ramp up Movement based on enemies hits
	MovementComponent->Acceleration = 800;
	MovementComponent->Deceleration = 800;
	MovementComponent->MaxSpeed = 2000.f;

	SwingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	SwingEffect->SetupAttachment(SceneComponent);
	

	ForceMultiplier = 1.5;
}

void AEnemyPaddle::BeginPlay()
{
	Super::BeginPlay();

	AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	if(MainGamemode)
	{
		MainGamemode->OnScoreUpdated.AddDynamic(this, &AEnemyPaddle::IncrementForceMultiplier);
	}
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

		// Max -176 Min -48
		RandomForce.X = (-32 * ForceMultiplier);

		// Based on position from center
		constexpr float MinYVal = -10.f;
		constexpr float MaxYVal = 10.f;
		RandomForce.Y = FMath::RandRange(MinYVal * (.5 + PercentageOfYDistanceFromCenter), MaxYVal * (.5 - PercentageOfYDistanceFromCenter));

		// Interpolation for Z
		const float MinZ =  (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(RandomForce.X, 3.f))) +
							(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(RandomForce.X, 2.f))) + (1.092 * RandomForce.X) + 54.61;
		
		const float MaxZ =  (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(RandomForce.X, 7.f))) + 
							(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(RandomForce.X, 6.f))) + 
							(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(RandomForce.X, 5.f))) + 
							(.0000373143f * FMath::Pow(RandomForce.X, 4.f)) + 
							(.00421209 * FMath::Pow(RandomForce.X, 3.f)) + 
							(0.290215 * FMath::Pow(RandomForce.X, 2.f)) + (11.9063 * RandomForce.X) + 262.738;

		RandomForce.Z = MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation);
		
		//UE_LOG(LogTemp, Warning, TEXT("RandomForce: %s"), *RandomForce.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Ball's Mesh Location: %s"), *BallInScene->BallMesh->GetComponentLocation().ToString());

		// Apply force
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
		SwingEffectCurrentRotation.Pitch +=180.0f;
		SwingEffectCurrentLocation.Y -= 48.0;
		SwingEffectCurrentRotation.Yaw += 30.0;
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw += 90.0f;
		SwingEffectCurrentRotation.Pitch -=180.0f;
		SwingEffectCurrentLocation.Y += 48.0;
		SwingEffectCurrentRotation.Yaw  -= 30.0;
	}
	SwingEffect->ResetSystem();
		
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
	this->SwingEffect->SetRelativeRotation(SwingEffectCurrentRotation,false,nullptr,ETeleportType::TeleportPhysics);

	this->SwingEffect->SetRelativeLocation(SwingEffectCurrentLocation,false,nullptr,ETeleportType::TeleportPhysics);

}

void AEnemyPaddle::IncrementForceMultiplier(int NewScore)
{
	if (ForceMultiplier <= 4.5)
	{
		ForceMultiplier += .1;
	}
	else
	{
		AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

		if(MainGamemode)
		{
			MainGamemode->OnScoreUpdated.Clear();
		}
	}
}