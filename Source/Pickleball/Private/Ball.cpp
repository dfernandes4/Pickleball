// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "BallPositionSymbol.h"
#include "EnemyAIController.h"
#include "EnemyPaddle.h"
#include "MainPlayerController.h"
#include "PlayerPaddle.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	Speed = 100;

	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(false);
	BallMesh->SetMassOverrideInKg(NAME_None, 0.048f, true);
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
	
	BallMesh->SetUseCCD(true);
	
	PlayerPaddle = Cast<APlayerPaddle>(GetWorld()->GetFirstPlayerController()->GetPawn());
	EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPaddle::StaticClass()));

	BallPositionSymbol = Cast<ABallPositionSymbol>(UGameplayStatics::GetActorOfClass(GetWorld(), ABallPositionSymbol::StaticClass()));

	BallLandingZ = 105.f;
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
		BallMesh->SetPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));
		BallMesh->PutRigidBodyToSleep();
		
		FTimerHandle ImpulseTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ImpulseTimerHandle, [this, Force]()
		{
			BallMesh->WakeRigidBody();
			BallMesh->AddImpulse(Force);
		}, .07, false);

		
			AudioComponent->SetSound(HitSound);
			AudioComponent->Play();
		
		
		CurrentPaddle = const_cast<APaddle*>(PaddleActor);
		
		FTimerHandle PredictProjectileLandingPointTimerHandle;
		FTimerDelegate PredictProjectileLandingPointTimerHandleTimerDel;
		PredictProjectileLandingPointTimerHandleTimerDel.BindUFunction(this, FName("PredictProjectileLandingPoint"));
		
		GetWorld()->GetTimerManager().SetTimer(PredictProjectileLandingPointTimerHandle, PredictProjectileLandingPointTimerHandleTimerDel, 0.1f, false);
		
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
	Params.StartLocation = BallMesh->GetComponentLocation();
	Params.LaunchVelocity = BallMesh->GetComponentVelocity();
	Params.bTraceWithCollision = true; // Set to true if you want to consider collisions along the path
	Params.ProjectileRadius = BallCollider->GetScaledSphereRadius(); // Set to the radius of your projectile
	Params.MaxSimTime = 4.f; // Maximum time in seconds for the simulation (adjust as needed)
	Params.TraceChannel = ECC_Visibility; // Trace channel to use for collision detection
	Params.SimFrequency = 30; // Frequency of path simulation points (higher values are more precise but more expensive
	Params.ActorsToIgnore.Add((this)); // Optional: ignore actors that you are sure will not hit

	//Params.DrawDebugType = EDrawDebugTrace::ForDuration;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, Params, PathResult);

	// The landing point
	if(PathResult.HitResult.IsValidBlockingHit())
	{
		BallPositionSymbol->SetActorLocation(PathResult.HitResult.Location);

		constexpr float Offset = 225;
		const FVector BallLandingPosition = BallPositionSymbol->GetActorLocation();
		FVector HittingLocation = BallLandingPosition + ( Offset * Params.LaunchVelocity.GetSafeNormal());
		HittingLocation.Z = -20.f;
		
		bDidBallLand = true;
		OnSwipeForceApplied(HittingLocation);
	}
	else
	{
		bDidBallLand = false;
	}
	
}

void ABall::OnSwipeForceApplied(const FVector& HittingLocation)
{
	if(bDidBallLand)
	{
		if(CurrentPaddle->IsA(APlayerPaddle::StaticClass()))
		{
			if(IsValid(EnemyPaddle))
			{
				Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState(HittingLocation);
				Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetBallLandingLocation(BallPositionSymbol->GetActorLocation());
			}
		}
		else if(CurrentPaddle->IsA(AEnemyPaddle::StaticClass()))
		{
			PlayerPaddle->SetIsPlayersTurn(true);
		}
	}
}

/*
FVector ABall::FindHittingLocation(bool bIsPlayerPaddle, const FVector& BallsVelocity, const TArray<FPredictProjectilePathPointData>& PathData) const
{
	float Min,Max;
	if(bIsPlayerPaddle)
	{
		Min = -215;
		Max = 115;
	}
	else
	{
		Min = -115;
		Max = 215;
	}
	
	Min = -115;
	Max = 115;
	const float XOffset = (FMath::RandBool() ? Min : Max);
	UE_LOG(LogTemp, Warning, TEXT("XOffset: %f"), XOffset);

	//Change if court size changes
	
	const FVector BallLandingPosition = BallPositionSymbol->GetActorLocation();
	FVector HittingLocation = BallLandingPosition + ( XOffset * BallsVelocity.GetSafeNormal());
	HittingLocation.Z = CalculateZPosition(PathData, HittingLocation.X);
	
	return HittingLocation;
}
*/