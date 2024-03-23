// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"

#include "Ball.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine.h"

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

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

void APaddle::StartSwing(float SwipeLength, const FVector& SwipeDirection, float SwipeTime)
{
	
	// If swipe is going up
	if((SwipeDirection.Y < 0) && !bIsSwingActive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SwingBeingCalled"));
		bIsSwingActive = true;
		
		//Set Values
		CurrentSwipeDirection = SwipeDirection;
		CurrentSwipeLength = SwipeLength;
		CurrentSwipeTime = SwipeTime;

		//Flip paddle after swipe
		FRotator NewRotation;
		if(!bIsFacingLeft)
		{
			bIsFacingLeft = true;
			NewRotation = FRotator(0.0f, GetActorRotation().Yaw + 180.0f, 0.0f);
		}
		else
		{
			bIsFacingLeft = false;
			NewRotation = FRotator(0.0f, GetActorRotation().Yaw - 180.0f, 0.0f);
		}
		
		this->SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);

		GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &APaddle::FinishSwing, .5f, false);
		//play the sound effect
		//play particle effect
		
	}
}

void APaddle::FinishSwing()
{
	bIsSwingActive = false;
}

void APaddle::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsSwingActive)
	{
		ABall* Ball = Cast<ABall>(OtherActor);
		if (Ball)
		{
			// Adjust force based on game's physics and gameplay feel
			const FVector Force = CurrentSwipeDirection.GetSafeNormal() * CurrentSwipeLength * ForceOfPaddle;
			const float Magnitude = FMath::Clamp((1.0f / CurrentSwipeTime) * ForceOfPaddle, MinForce, MaxForce);
			Ball->ApplySwipeForce(Force * Magnitude);
		}
	}
}



