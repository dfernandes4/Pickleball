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

	SwingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	SwingEffect->SetupAttachment(SceneComponent);
	
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

	if (MainGamemode != nullptr)
	{
		MainGamemode->OnGameOver.AddDynamic(this, &APlayerPaddle::SavePlayerHighScore);
	}
	LoadGame();
}

void APlayerPaddle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveAllStats();
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
				FVector ScaledPaddleVelocity = (Cast<AMainPlayerController>(GetController())->GetPaddleVelocity());
				ScaledPaddleVelocity.X *= .02;
				ScaledPaddleVelocity.Y *= .01;

				UE_LOG(LogTemp, Warning, TEXT("Paddle Velocity: %s"), *(Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()).ToString());
				UE_LOG(LogTemp, Warning, TEXT("Paddle Scaled Velocity: %s"), *(ScaledPaddleVelocity.ToString()));

				constexpr float FarthestHittingLocation = -895.0f;
				const float PercentageOfXDistanceFromFarthestHittingLocation = FMath::Clamp(GetActorLocation().X / FarthestHittingLocation, 0,1);
				
				const float ForceXDistance = FMath::Clamp(ScaledPaddleVelocity.X, 32, 176);
				const float ForceYDistance = FMath::Clamp(ScaledPaddleVelocity.Y, -15.0f, 15.0f);
				
				// Interpolation for Z
				const float MinZ =  (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(-ForceXDistance, 3.f))) +
									(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(-ForceXDistance, 2.f))) + (1.092 * -ForceXDistance) + 54.61;
		
				const float MaxZ =  (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(-ForceXDistance, 7.f))) + 
									(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(-ForceXDistance, 6.f))) + 
									(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(-ForceXDistance, 5.f))) + 
									(.0000373143f * FMath::Pow(-ForceXDistance, 4.f)) + 
									(.00421209 * FMath::Pow(-ForceXDistance, 3.f)) + 
									(0.290215 * FMath::Pow(-ForceXDistance, 2.f)) + (11.9063 * -ForceXDistance) + 262.738;

				const float ForceZDistance = .5 * (MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation));
				
				const FVector Force = FVector(ForceXDistance, ForceYDistance, ForceZDistance);
				UE_LOG(LogTemp, Warning, TEXT("Force: %s"), *Force.ToString());
				
				CurrentScore++;

				if(CurrentScore > HighScore)
				{
					HighScore = CurrentScore;
				}
				
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
	FRotator SwingEffectCurrentRotation = SwingEffect->GetRelativeRotation();
	FVector  SwingEffectCurrentLocation = SwingEffect->GetRelativeLocation();
	FRotator CurrentRotation = PaddleSprite->GetRelativeRotation();
	if(!bIsFacingLeft)
	{
		bIsFacingLeft = true;
		CurrentRotation.Yaw -= 90.0f;
		SwingEffectCurrentRotation.Pitch +=180.0f;
		SwingEffectCurrentLocation.Y -= 48.0;
		SwingEffectCurrentRotation.Yaw -= 30.0;
		
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw += 90.0f;
		SwingEffectCurrentRotation.Pitch -=180.0f;
		SwingEffectCurrentLocation.Y += 48.0;
		SwingEffectCurrentRotation.Yaw  += 30.0;
	}
	SwingEffect->ResetSystem();
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
	this->SwingEffect->SetRelativeRotation(SwingEffectCurrentRotation,false,nullptr,ETeleportType::TeleportPhysics);

	this->SwingEffect->SetRelativeLocation(SwingEffectCurrentLocation,false,nullptr,ETeleportType::TeleportPhysics);
}

void APlayerPaddle::SaveAllStats()
{
	UPickleballSaveGame* SaveGameInstance = GetOrCreateSaveGame();
	
	SaveGameInstance->SetPlayerCoins(CurrentCoinCount);
	SaveGameInstance->SetPlayerHighScore(HighScore);
	SaveGameInstance->SetPaddleUnlockStatuses(PaddleUnlockStatuses);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerCoins()
{
	UPickleballSaveGame* SaveGameInstance = GetOrCreateSaveGame();
	
	SaveGameInstance->SetPlayerCoins(CurrentCoinCount);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerHighScore()
{
	UPickleballSaveGame* SaveGameInstance = GetOrCreateSaveGame();
	
	SaveGameInstance->SetPlayerHighScore(HighScore);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::SavePlayerPaddleUnlockStatuses()
{
	UPickleballSaveGame* SaveGameInstance = GetOrCreateSaveGame();
	
	SaveGameInstance->SetPaddleUnlockStatuses(PaddleUnlockStatuses);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSaveSlot", 0);
}

void APlayerPaddle::LoadGame()
{
	UPickleballSaveGame* SaveGameInstance = GetOrCreateSaveGame();

	if(SaveGameInstance != nullptr)
	{
		CurrentCoinCount = SaveGameInstance->GetPlayerCoins();
		HighScore = SaveGameInstance->GetPlayerHighScore();
		PaddleUnlockStatuses = SaveGameInstance->GetPaddleUnlockStatuses();
	}
}

int APlayerPaddle::GetHighScore() const
{
	return HighScore;
}

int APlayerPaddle::GetPlayerCoins() const
{
	return CurrentCoinCount;
}

UPickleballSaveGame* APlayerPaddle::GetOrCreateSaveGame()
{
	UPickleballSaveGame* SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UPickleballSaveGame>(UGameplayStatics::CreateSaveGameObject(UPickleballSaveGame::StaticClass()));
	}
	return SaveGameInstance;
}


