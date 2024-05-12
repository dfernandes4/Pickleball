// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"
#include "NiagaraComponent.h"
#include "Ball.h"
#include "MainGamemode.h"
#include "MainPlayerController.h"
#include "PaperSpriteComponent.h"
#include "PickleballSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bIsFirstSwing = true;

	CurrentScore = 0;

	bIsPlayersTurn = true;

	SwipeForceMultiplier = .02f;

	SwingEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("swingeffect"));
	AttachToComponent(PaddleSprite, FAttachmentTransformRules::KeepRelativeTransform);
	

	HitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	HitEffect->SetupAttachment(PaddleSprite);
	
}

void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnPurchaseCompleted.AddDynamic(this, &APlayerPaddle::AddPlayerCoins);
	}
}

void APlayerPaddle::StartSwing(const FVector& BallCurrentLocation)
{
	// If swipe is going up
	if(bIsPlayersTurn)
	{
		bIsPlayersTurn = false;

		FlipPaddle();
		
		//play particle effect
		
		if(bIsInHittingZone)
		{
			if (IsValid(BallInScene))
			{
				FVector ScaledPaddleVelocity = (Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()) * SwipeForceMultiplier;

				UE_LOG(LogTemp, Warning, TEXT("Paddle Velocity: %s"), *(Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()).ToString());
				
				//Range of X-Y-Z Should be between 10-40
				float MinXOffset = 25 * (BallCurrentLocation.X / -670);
				float MaxXOffset = 10 * (BallCurrentLocation.X / -670);;
				const float ForceXDistance = FMath::Clamp(ScaledPaddleVelocity.X, 15.0f + MinXOffset, 30.0f + MaxXOffset);
				const float ForceYDistance = FMath::Clamp(ScaledPaddleVelocity.Y, -10.0f, 10.0f);
				float ForceZDistance = 20;
				if(BallCurrentLocation.Z < 185)
				{
					// 10 is the addition needed to make the ball go over the fence if it is on the floor
					ForceZDistance += (15 * (BallCurrentLocation.Z / 185));
					if(BallCurrentLocation.X < -470 || ForceYDistance > 2 || ForceYDistance < -2)
					{
						ForceZDistance += 2;
					}
					{
						ForceZDistance += 5;
					}
				}
				
				// Normalize the swipe direction to use as a direction vector in the world
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				UE_LOG(LogTemp, Warning, TEXT("Force: %s"), *Force.ToString());
				
				CurrentScore++;
				
				if(MainGamemode)
				{
					MainGamemode->OnScoreUpdated.Broadcast(CurrentScore);
				}
				
				BallInScene->ApplySwipeForce(Force, this);
				
			}
		}
		
	}
}

void APlayerPaddle::AddPlayerCoins(int32 CoinsAmount)
{
	CurrentCoinCount += CoinsAmount;
}

float APlayerPaddle::GetScore() const
{
	return CurrentScore;
}

void APlayerPaddle::OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPaddleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor->IsA(ABall::StaticClass()))
	{
		StartSwing(Cast<ABall>(OtherActor)->BallMesh->GetComponentLocation());
	}
}

void APlayerPaddle::SetIsPlayersTurn(bool bIsPlayersTurnIn)
{
	bIsPlayersTurn = bIsPlayersTurnIn;
}

void APlayerPaddle::FlipPaddle()
{
	//Flip paddle after swipe
	FRotator CurrentRotation = PaddleSprite->GetRelativeRotation();
	if(!bIsFacingLeft)
	{
		bIsFacingLeft = true;
		CurrentRotation.Yaw += 90.0f;
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw -= 90.0f;
	}
		
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
}

void APlayerPaddle::SaveAllStats()
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
	}
	
	SaveGameInstance->SetPlayerCoins(CurrentCoinCount);
	SaveGameInstance->SetPlayerHighScore(HighScore);
	SaveGameInstance->SetPaddleUnlockStatuses(PaddleUnlockStatuses);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerCoins(int32 CoinAmount)
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
	}
	
	SaveGameInstance->SetPlayerCoins(CoinAmount);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerHighScore(int32 HighScoreAmount)
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
	}
	
	SaveGameInstance->SetPlayerHighScore(HighScoreAmount);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerPaddleUnlockStatuses(const TMap<FString, bool>& PaddleUnlockStatusesIn)
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
	}
	
	SaveGameInstance->SetPaddleUnlockStatuses(PaddleUnlockStatusesIn);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::LoadGame()
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

	if(SaveGameInstance != nullptr)
	{
		CurrentCoinCount = SaveGameInstance->GetPlayerCoins();
		HighScore = SaveGameInstance->GetPlayerHighScore();
		PaddleUnlockStatuses = SaveGameInstance->GetPaddleUnlockStatuses();
	}
}


