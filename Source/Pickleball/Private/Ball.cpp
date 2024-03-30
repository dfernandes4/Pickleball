// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "BallPositionSymbol.h"
#include "EnemyAIController.h"
#include "EnemyPaddle.h"
#include "MainPlayerController.h"
#include "PlayerPaddle.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

class AEnemyAIController;
// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SceneComponent);
	
	
	BallCollider = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollider"));
	BallCollider->SetupAttachment(BallMesh);

	Speed = 100;

	BallMesh->SetSimulatePhysics(true);
	
	BallMesh->SetEnableGravity(true);
	BallMesh->SetMassOverrideInKg(NAME_None, 0.048f, true);
	BallCollider->SetCollisionProfileName(TEXT("Custom"));

	// Alternatively, you can set specific collision responses
	BallCollider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallCollider->BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
	BallCollider->BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
}

void ABall::ApplySwipeForce(const FVector& Force)	
{
	if(IsValid(BallMesh))
	{
		BallMesh->AddImpulse(Force);
		UE_LOG(LogTemp, Warning, TEXT("Applying force: %s"), *Force.ToString());
	}
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(OtherActor->IsA(APlayerPaddle::StaticClass()))
	{
		//Set to Responding state
		TObjectPtr<AEnemyPaddle> EnemyPaddle {Cast<AEnemyPaddle>(OtherActor)};
		if(IsValid(EnemyPaddle.Get()))
		{
			//Add in the location to hit at
			const float BeginningOfCourtAfterKitchen = 236;
			const float EndOfCourt = 730;
			
			const FVector HittingLocation = FindHittingLocation(BeginningOfCourtAfterKitchen, EndOfCourt);
			Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState(HittingLocation);
		}
	}
	else if(OtherActor->IsA(AEnemyPaddle::StaticClass()))
	{
		BallPositionSymbol->SetActorHiddenInGame(false);

		if(IsValid(APlayerPaddle.Get()))
		{
			const float BeginningOfCourtAfterKitchen = -236;
			const float EndOfCourt = -730;

			const FVector HittingLocation = FindHittingLocation(BeginningOfCourtAfterKitchen, EndOfCourt);
			Cast<AMainPlayerController>(PlayerPaddle->GetController())->MoveToZone(HittingLocation);
		}
	}
	else
	{
		BallPositionSymbol->SetActorHiddenInGame(true);
	}
	
	// can Reflect the ball's direction and modify speed
	//if not paddle play floor sound
}

void ABall::PredictProjectileLandingPoint(const FVector& StartLocation, const FVector& LaunchVelocity)
{
	FPredictProjectilePathParams Params;
	Params.StartLocation = StartLocation;
	Params.LaunchVelocity = LaunchVelocity;
	Params.bTraceWithCollision = true; // Set to true if you want to consider collisions along the path
	Params.ProjectileRadius = 1.0f; // Set to the radius of your projectile
	Params.MaxSimTime = 1.0f; // Maximum time in seconds for the simulation (adjust as needed)
	Params.TraceChannel = ECC_Visibility; // Trace channel to use for collision detection
	Params.SimFrequency = 0.25; // Frequency of path simulation points (lower values are more precise but more expensive)
	Params.DrawDebugType = EDrawDebugTrace::None; // Optional: draw the predicted path for debugging
	Params.bTraceWithChannel = true;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, Params, PathResult);

	// The landing point
	if(PathResult.HitResult.IsValidBlockingHit())
	{
		BallPositionSymbol->SetActorLocation(PathResult.HitResult.Location);
	}
}

const FVector& ABall::FindHittingLocation(const float BeginningOfCourtAfterKitchen, const float EndOfCourt) const
{
	constexpr float MIN = -80.0f;
	constexpr float MAX = 80.0f;
	const float XOffset = FMath::RandRange(MIN, MAX);
	const float YOffset = FMath::RandRange(MIN, MAX);

	//Change if court size changes
	constexpr float LeftSideOfCourt = -365.0f;
	constexpr float RightSideOfCourt = 365.0f;
	
	const FVector BallLandingPosition = BallPositionSymbol->GetActorLocation();
	const float HittingLocationX = FMath::Clamp(BallLandingPosition.X + XOffset, BeginningOfCourtAfterKitchen, EndOfCourt);
	const float HittingLocationY = FMath::Clamp(BallLandingPosition.Y + YOffset, LeftSideOfCourt, RightSideOfCourt);
	const FVector HittingLocation = FVector(HittingLocationX, HittingLocationY, 1);
	
	return HittingLocation;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

