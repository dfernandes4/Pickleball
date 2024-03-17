// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"

#include "Ball.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
APaddle::APaddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	PaddleSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaddleSprite"));
	PaddleSprite->SetupAttachment(SceneComponent);
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(PaddleSprite);

	BoxCollider->OnComponentHit.AddDynamic(this, &APaddle::OnBallHit);

	ForceOfPaddle = 100.0f;
}

void APaddle::Swing(float SwipeLength, const FVector& SwipeDirection, float SwipeTime)
{
	// If swipe is going up
	if((SwipeDirection.Y < 0) && !bIsSwingActive)
	{
		CurrentSwipeDirection = SwipeDirection;
		CurrentSwipeLength = SwipeLength;
		CurrentSwipeTime = SwipeTime;
		bIsSwingActive = true;

		//Teleport paddle to be flipped and track the area that would have been swiped
			//After finished make bIsSwingActive = false
		//play the sound effect
		
	}
}

void APaddle::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsSwingActive)
	{
		ABall* Ball = Cast<ABall>(OtherActor);
		if (Ball)
		{
			// Adjust force based on your game's physics and gameplay feel
			const FVector Force = CurrentSwipeDirection.GetSafeNormal() * CurrentSwipeLength * ForceOfPaddle; // Define ForceMultiplier as needed
			const float Magnitude = FMath::Clamp((1.0f / CurrentSwipeTime) * ForceOfPaddle, MinForce, MaxForce);
			Ball->ApplySwipeForce(Force * Magnitude);
		}
	}
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

