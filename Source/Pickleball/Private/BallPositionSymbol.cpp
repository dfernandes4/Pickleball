// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPositionSymbol.h"

// Sets default values
ABallPositionSymbol::ABallPositionSymbol()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("XSymbolSprite"));
	SpriteComponent->SetupAttachment(SceneComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneComponent);
}

void ABallPositionSymbol::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
}
