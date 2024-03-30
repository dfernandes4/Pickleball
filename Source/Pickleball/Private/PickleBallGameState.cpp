// Fill out your copyright notice in the Description page of Project Settings.


#include "PickleBallGameState.h"

#include "EnemyPaddle.h"
#include "PlayerPaddle.h"
#include "Kismet/GameplayStatics.h"

APickleBallGameState::APickleBallGameState()
{
}

void APickleBallGameState::BeginPlay()
{
	Super::BeginPlay();

	PlayerPaddle = Cast<APlayerPaddle>(GetWorld()->GetFirstPlayerController()->GetPawn());
	EnemyPaddle = Cast<AEnemyPaddle>(UGameplayStatics::GetActorOfClass(GetWorld(), EnemyPaddleClass));
}
