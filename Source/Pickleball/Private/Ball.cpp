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
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SceneComponent);
	
	
	BallCollider = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollider"));
	BallCollider->SetupAttachment(BallMesh);

	Speed = 100;

	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(false);
	BallMesh->SetMassOverrideInKg(NAME_None, 0.048f, true);
	BallMesh->SetUseCCD(true);
	BallCollider->SetCollisionProfileName(TEXT("Custom"));

	// Alternatively, you can set specific collision responses
	BallCollider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallCollider->BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
	BallCollider->BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	// Get PlayerPaddle from GameStateClass
	
	PlayerPaddle = Cast<APlayerPaddle>(GetWorld()->GetFirstPlayerController()->GetPawn());
	EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPaddle::StaticClass()));

	BallPositionSymbol = Cast<ABallPositionSymbol>(UGameplayStatics::GetActorOfClass(GetWorld(), ABallPositionSymbol::StaticClass()));
}

void ABall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABall::ApplySwipeForce(const FVector& Force, const APaddle* PaddleActor)	
{
	if(IsValid(BallMesh))
	{
		BallMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
		BallMesh->AddImpulse(Force);
		
		FTimerHandle PredictProjectileLandingPointTimerHandle;
		FTimerDelegate PredictProjectileLandingPointTimerHandleTimerDel;
		PredictProjectileLandingPointTimerHandleTimerDel.BindUFunction(this, FName("PredictProjectileLandingPoint"));
		
		GetWorld()->GetTimerManager().SetTimer(PredictProjectileLandingPointTimerHandle, PredictProjectileLandingPointTimerHandleTimerDel, 0.01f, false);
		
		CurrentPaddle = const_cast<APaddle*>(PaddleActor);
		
		UE_LOG(LogTemp, Warning, TEXT("Applying force: %s"), *Force.ToString());
	}
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->ActorHasTag("Court"))
	{
		if(!BallPositionSymbol->IsHidden())
		{
			BallPositionSymbol->SetActorHiddenInGame(true);
		}
	}
	// can Reflect the ball's direction and modify speed
	//if not paddle play floor sound
}

void ABall::PredictProjectileLandingPoint()
{
	FPredictProjectilePathParams Params;
	Params.StartLocation = GetActorLocation();
	Params.LaunchVelocity = BallMesh->GetComponentVelocity();
	Params.bTraceWithCollision = true; // Set to true if you want to consider collisions along the path
	Params.ProjectileRadius = BallCollider->GetScaledSphereRadius(); // Set to the radius of your projectile
	Params.MaxSimTime = 4.0f; // Maximum time in seconds for the simulation (adjust as needed)
	Params.TraceChannel = ECC_Visibility; // Trace channel to use for collision detection
	Params.SimFrequency = 30; // Frequency of path simulation points (higher values are more precise but more expensive
	//Params.DrawDebugType = EDrawDebugTrace::ForDuration;
	Params.ActorsToIgnore.Add((this)); // Optional: ignore actors that you are sure will not hit

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, Params, PathResult);

	// The landing point
	if(PathResult.HitResult.IsValidBlockingHit())
	{
		BallPositionSymbol->SetActorLocation(PathResult.HitResult.Location);
	}
	bDidBallLand = PathResult.HitResult.IsValidBlockingHit();
	OnSwipeForceApplied();
}

void ABall::OnSwipeForceApplied() const 
{
	if(bDidBallLand)
	{
		if(CurrentPaddle->IsA(APlayerPaddle::StaticClass()))
		{
			if(IsValid(EnemyPaddle))
			{
				constexpr float BeginningOfCourtAfterKitchen = 243;
				constexpr float EndOfCourt = 730;
			
				const FVector HittingLocation = FindHittingLocation(BeginningOfCourtAfterKitchen, EndOfCourt);
				Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState(HittingLocation);
			}
		}
		else if(CurrentPaddle->IsA(AEnemyPaddle::StaticClass()))
		{
			BallPositionSymbol->SetActorHiddenInGame(false);

			if(IsValid(PlayerPaddle))
			{
				constexpr float BeginningOfCourtAfterKitchen = -243;
				constexpr float EndOfCourt = -730;

				const FVector HittingLocation = FindHittingLocation(BeginningOfCourtAfterKitchen, EndOfCourt);
				Cast<AMainPlayerController>(PlayerPaddle->GetController())->MoveToZone(HittingLocation);
			}
		}
	}
}

FVector ABall::FindHittingLocation(const float BeginningOfCourtAfterKitchen, const float EndOfCourt) const
{
	constexpr float MIN = -80.0f;
	constexpr float MAX = 80.0f;
	const float XOffset = (FMath::RandBool() ? MAX : MIN);
	UE_LOG(LogTemp, Warning, TEXT("XOffset: %f"), XOffset);

	//Change if court size changes
	constexpr float LeftSideOfCourt = -370.0f;
	constexpr float RightSideOfCourt = 370.0f;
	
	const FVector BallLandingPosition = BallPositionSymbol->GetActorLocation();
	const float HittingLocationX = (BallLandingPosition.X + XOffset);
	const float HittingLocationY = BallLandingPosition.Y;
	const FVector HittingLocation = FVector(HittingLocationX, HittingLocationY, 1);
	
	return HittingLocation;
}

