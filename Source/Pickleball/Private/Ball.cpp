// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

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
}

void ABall::ApplySwipeForce(const FVector& Force) const	
{
	BallCollider->AddForce(Force);
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// can Reflect the ball's direction and modify speed
	//if not paddle play floor sound
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

