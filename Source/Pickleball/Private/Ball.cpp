// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "EnemyAIController.h"
#include "EnemyPaddle.h"
#include "PlayerPaddle.h"
#include "Components/SphereComponent.h"

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
			//Cast<AEnemyAIController>(EnemyPaddle->GetController())->SetRespondingState();
		}
	}
	else if(OtherActor->IsA(AEnemyPaddle::StaticClass()))
	{
		// Display the X on players side
	}
	
	// can Reflect the ball's direction and modify speed
	//if not paddle play floor sound
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

