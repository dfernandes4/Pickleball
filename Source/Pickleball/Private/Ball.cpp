// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "NiagaraComponent.h"
#include "BallPositionSymbol.h"
#include "EnemyAIController.h"
#include "EnemyPaddle.h"
#include "MainGamemode.h"
#include "PlayerPaddle.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
	
	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	Trail->SetupAttachment(BallMesh);
    
	bIsGameOver = false;
	bCanBallHitInKitchen = true;
	
	Speed = 100;

	CurrentBounceCount = 0;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

    BallMesh->SetUseCCD(true);
        
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetEnableGravity(false);
    BallMesh->SetMassOverrideInKg(NAME_None, 0.06f, true);
    BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnBallHit);
    
    BallCollider->SetCollisionProfileName(TEXT("Custom"));

    // Alternatively, you can set specific collision responses
    BallCollider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallCollider->BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
    BallCollider->BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    
    PlayerPaddle = Cast<APlayerPaddle>(GetWorld()->GetFirstPlayerController()->GetPawn());
    EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPaddle::StaticClass()));

    BallPositionSymbol = Cast<ABallPositionSymbol>(UGameplayStatics::GetActorOfClass(GetWorld(), ABallPositionSymbol::StaticClass()));

    BallLandingZ = 105.f;
    
    MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());
    MainGamemode->OnGameOver.AddDynamic(this, &ABall::OnGameOver);

}

void ABall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABall::ApplySwipeForce(const FVector& Force, const APaddle* PaddleActor)	
{
	if(IsValid(BallMesh))
	{
		BallMesh->SetWorldLocation(FVector(BallMesh->GetComponentLocation().X, BallMesh->GetComponentLocation().Y, 45));
		
		BallMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
		BallMesh->SetPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));
		BallMesh->PutRigidBodyToSleep();
		
		FTimerHandle ImpulseTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ImpulseTimerHandle, [this, Force]()
		{
			BallMesh->WakeRigidBody();
			BallMesh->AddImpulse(Force);
		}, .08, false);
		
		
		
		CurrentPaddle = const_cast<APaddle*>(PaddleActor);
		FTimerHandle PredictProjectileLandingPointTimerHandle;
		FTimerDelegate PredictProjectileLandingPointTimerHandleTimerDel;
		PredictProjectileLandingPointTimerHandleTimerDel.BindUFunction(this, FName("PredictProjectileLandingPoint"));

		if(CurrentPaddle->IsA(APlayerPaddle::StaticClass()))
		{
			GetWorld()->GetTimerManager().SetTimer(PredictProjectileLandingPointTimerHandle, PredictProjectileLandingPointTimerHandleTimerDel, 0.1f, false);
		}
		else if(CurrentPaddle->IsA(AEnemyPaddle::StaticClass()))
		{
			OnSwipeForceApplied();
		}
	}
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIsGameOver)
	{
		MainGamemode->OnGameOver.Broadcast();
	}
	else
	{
		if(OtherActor->ActorHasTag("Court") || OtherActor->ActorHasTag("Background") || OtherActor->ActorHasTag("Dark"))
		{
			CurrentBounceCount++;
			// Each Bounce + 2 to the count ...?
			if(CurrentBounceCount > 1)
			{
				MainGamemode->OnGameOver.Broadcast();
			}

			if(BounceEffect != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(),BounceSound);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BounceEffect, BallMesh->GetComponentLocation());
			}
        
			FVector BallLandingLocation = BallPositionSymbol->GetActorLocation();
			if(IsValid(EnemyPaddle) && (BallLandingLocation.X > -8 && BallLandingLocation.X < 680 && BallLandingLocation.Y > -304 && BallLandingLocation.Y < 304) && EnemyPaddle->GetIsEnemiesTurn())
			{
				Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetBallLandingLocation(BallLandingLocation);
				Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState(BallLandingLocation);
			}
		}
	}
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
	Params.SimFrequency = 20; // Frequency of path simulation points (higher values are more precise but more expensive
	Params.ActorsToIgnore.Add((this)); // Optional: ignore actors that you are sure will not hit

	//Params.DrawDebugType = EDrawDebugTrace::ForDuration;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, Params, PathResult);

	// The landing point
	if(PathResult.HitResult.IsValidBlockingHit())
	{
		BallPositionSymbol->SetActorLocation(PathResult.HitResult.Location);

		float Offset = 225;
		const FVector BallLandingPosition = BallPositionSymbol->GetActorLocation();
		if(BallLandingPosition.X >= -9.5f && BallLandingPosition.X <= 9.5f)
		{
			Offset *= 2.f;
		}
		FVector HittingLocation = BallLandingPosition + ( Offset * Params.LaunchVelocity.GetSafeNormal());
		HittingLocation.Z = 0.f;
		
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
		UE_LOG(LogTemp, Warning, TEXT("Ball's Landing Location: %s"), *BallPositionSymbol->GetActorLocation().ToString());
		
		if(CurrentPaddle->IsA(APlayerPaddle::StaticClass()))
		{
			// Check if landing location is valid 
			FVector BallLandingLocation = BallPositionSymbol->GetActorLocation();
			if(IsValid(EnemyPaddle) && (BallLandingLocation.X > -8 && BallLandingLocation.X < 680 && BallLandingLocation.Y > -304 && BallLandingLocation.Y < 304))
			{
				// Check if they can hit in kitchen
				bCanBallHitInKitchen = CanBallHitInKitchen();
				if(!bCanBallHitInKitchen)
				{
					bIsGameOver = true;
				}
				else
				{
					EnemyPaddle->SetIsEnemiesTurn(true);
					Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetBallLandingLocation(BallLandingLocation);
					Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState(HittingLocation);
					EnemyPaddle->AdjustEnemySpeed(BallMesh->GetComponentVelocity(), HittingLocation);
				}
			}
			else
			{
				bIsGameOver = true;
			}
		}
		else if(CurrentPaddle->IsA(AEnemyPaddle::StaticClass()))
		{
			PlayerPaddle->SetIsPlayersTurn(true);
		}
	}
	CurrentBounceCount = 0;
}

int32 ABall::GetCurrentBounceCount() const
{
	return CurrentBounceCount;
}

void ABall::OnGameOver()
{
    BallMesh->OnComponentHit.Clear();
	//Do something on restart to rebind the hit event
}

bool ABall::CanBallHitInKitchen()
{
	FVector BallLocation = BallMesh->GetComponentLocation();
	bool bIsBallInKitchen =  (BallLocation.X > -190.0f && BallLocation.Y > -260.0f && BallLocation.Y < 260.0f);
	return !(bIsBallInKitchen && CurrentBounceCount == 0);
}

bool ABall::GetCanBallHitInKitchen()
{
	return bCanBallHitInKitchen;
}
