// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"

#include "Ball.h"
#include "MainGamemode.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APaddle::APaddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	PaddleSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaddleSprite"));
	PaddleSprite->SetupAttachment(SceneComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowDirection"));
	ArrowComponent->SetupAttachment(SceneComponent);
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComponent);

	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APaddle::OnPaddleBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &APaddle::OnPaddleEndOverlap);

	bIsFirstSwing = true;
	bIsFacingLeft = true;

	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(MainGamemode)
	{
		MainGamemode->OnGameOver.AddDynamic(this, &APaddle::OnAnyPaddleGameOver);
	}
}

void APaddle::BeginPlay()
{
	Super::BeginPlay();
	BallInScene = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
}

void APaddle::OnAnyPaddleGameOver()
{
	BoxCollider->Activate(false);
	BoxCollider->OnComponentBeginOverlap.RemoveDynamic(this, &APaddle::OnPaddleBeginOverlap);
	BoxCollider->OnComponentEndOverlap.RemoveDynamic(this, &APaddle::OnPaddleEndOverlap);
}

void APaddle::OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ABall::StaticClass()))
	{
		bIsInHittingZone = true;
	}
}

void APaddle::OnPaddleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(ABall::StaticClass()) && !bIsFirstSwing)
	{
		bIsInHittingZone = false;
	}
}

bool APaddle::GetIsInHittingZone() const
{
	return bIsInHittingZone;
}

bool APaddle::GetIsFirstSwing() const
{
	return bIsFirstSwing;
}


