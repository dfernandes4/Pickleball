// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"
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
	
}

void APaddle::Swing(float SwipeLength, FVector SwipeDirection, float SwipeTime)
{
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

