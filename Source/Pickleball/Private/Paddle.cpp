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

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APaddle::OnPaddleBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &APaddle::OnPaddleEndOverlap);

	ForceOfPaddle = 500.0f;
	bIsInHittingZone = false;
	BallInScene = nullptr;
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
	BallInScene = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
}

void APaddle::StartSwing(float ScreenYDistance, float ScreenXDistance, float SwipeTime)
{
	
	// If swipe is going up
	if((ScreenYDistance < 0) && !bIsSwingActive)
	{
		bIsSwingActive = true;

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
		
		if(bIsInHittingZone)
		{
			if (IsValid(BallInScene))
			{
				const float RelativeAdjustmentToWorld = 0.075;
				
				//Range of X-Y-Z Should be between 10-40
				const float ForceXDistance = FMath::Clamp(((-ScreenYDistance * RelativeAdjustmentToWorld) / (SwipeTime * 2.5)), 20.0f, 50.0f);
				const float ForceYDistance = FMath::Clamp(ScreenXDistance * RelativeAdjustmentToWorld, -75.0f, 75.0f);
				const float ForceZDistance = FMath::Clamp(-ScreenYDistance * RelativeAdjustmentToWorld, 30.0f, 75.0f);
				
				// Normalize the swipe direction to use as a direction vector in the world
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				
				// Apply the force in the calculated direction with the calculated magnitude
				BallInScene->ApplySwipeForce(Force);
				
			}
		}
		
	}
}

void APaddle::FinishSwing()
{
	bIsSwingActive = false;
}

void APaddle::OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsInHittingZone = true;
}

void APaddle::OnPaddleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsInHittingZone = false;
}

