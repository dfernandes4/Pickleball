// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"
#include "NiagaraComponent.h"
#include "Ball.h"
#include "MainGamemode.h"
#include "MainPlayerController.h"
#include "PaddleInfo.h"
#include "PaperSpriteComponent.h"
#include "PickleBallGameInstance.h"
#include "PickleballSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bIsFirstSwing = true;

	CoinMultiplier = 1;
	CurrentCoinCount = 0;
	CoinsEarnedFromLastMatch = 0;
	HighScore = 0;
	CurrentScore = 0;
    LastScore = 0;
	PaddleUnlockStatuses = {
		// Common
		{FName("Green"), true},
		{FName("Orange"), false},
		{FName("Purple"), false},
		{FName("Red"), false},
		{FName("Yellow"), false},
		{FName("Blue"), false},
		{FName("Pride"), false},
		// Rare
		{FName("Webbed"), false},
		{FName("Camo"), false},
        {FName("Smiley"), false},
		{FName("Grumpy"), false},
		{FName("The_Patriot"), false},
		{FName("Pizza"), false},
		// Epic
		{FName("Doggy"), false},
		{FName("Whiskers"), false},
		{FName("Bonehead"), false},
		{FName("Lovestruck"), false},
		// Legendary
		{FName("Tomahawk"), false},
		{FName("Rattle"), false},
		{FName("Shredder"), false},
		{FName("Mjolnir"), false},
		// Mythic
		{FName("Money_Bag"), false},
		{FName("Punchin_"), false},
		{FName("Pickle"), false}
	};
	
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
		PlayerController->OnPurchaseCompleted.AddDynamic(this, &APlayerPaddle::AddCoins);
	}
	
	if (MainGamemode != nullptr)
	{
		MainGamemode->OnGameOver.AddDynamic(this, &APlayerPaddle::OnGameOver);
	}
    
    
    PickleBallGameInstance = Cast<UPickleBallGameInstance>(GetGameInstance());
    if(PickleBallGameInstance->GetIsGameLoaded())
    {
        OnGameLoaded();
    }
    else
    {
        PickleBallGameInstance->LoadFinished.AddDynamic(this, &APlayerPaddle::OnGameLoaded);
    }
    
}

void APlayerPaddle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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
                UGameplayStatics::PlaySound2D(GetWorld(),PaddleSoundEffect);
				
				FVector ScaledPaddleVelocity = (Cast<AMainPlayerController>(GetController())->GetPaddleVelocity());
				ScaledPaddleVelocity.X *= .02;
				ScaledPaddleVelocity.Y *= .01;

				UE_LOG(LogTemp, Warning, TEXT("Paddle Velocity: %s"), *(Cast<AMainPlayerController>(GetController())->GetPaddleVelocity()).ToString());
				UE_LOG(LogTemp, Warning, TEXT("Paddle Scaled Velocity: %s"), *(ScaledPaddleVelocity.ToString()));

				constexpr float FarthestHittingLocation = -895.0f;
				const float PercentageOfXDistanceFromFarthestHittingLocation = FMath::Clamp(GetActorLocation().X / FarthestHittingLocation, 0,1);

				const float ForceYDistance = FMath::Clamp(ScaledPaddleVelocity.Y, -15.0f, 15.0f);

				float ForceXDistance = 0;
				float ForceZDistance = 0;
				
				if(ScaledPaddleVelocity.X < 5)
				{
					ForceXDistance = 10;
					ForceZDistance = ForceXDistance;
				}
				else
				{
					ForceXDistance = FMath::Clamp(ScaledPaddleVelocity.X, 32, 176);
				
					// Interpolation for Z
					const float MinZ =  (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(-ForceXDistance, 3.f))) +
										(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(-ForceXDistance, 2.f))) + (1.092 * -ForceXDistance) + 54.61;
	
					const float MaxZ =  (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(-ForceXDistance, 7.f))) + 
										(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(-ForceXDistance, 6.f))) + 
										(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(-ForceXDistance, 5.f))) + 
										(.0000373143f * FMath::Pow(-ForceXDistance, 4.f)) + 
										(.00421209 * FMath::Pow(-ForceXDistance, 3.f)) + 
										(0.290215 * FMath::Pow(-ForceXDistance, 2.f)) + (11.9063 * -ForceXDistance) + 262.738;
				
					ForceZDistance = .5 * (MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation));
				}
				
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

void APlayerPaddle::OnGameOver()
{
	if(CurrentScore > HighScore)
	{
		HighScore = CurrentScore;
		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			//PlayerController->SubmitHighscore(HighScore, FName("HS"));
		}
	}
    
    int32 LastHundredsCount = FMath::FloorToInt(LastScore / 100.f);
    int32 LastThousandsCount = FMath::FloorToInt(LastScore / 1000.f);
    
	int32 HundredsCount = FMath::FloorToInt(CurrentScore / 100.f);
	int32 ThousandsCount = FMath::FloorToInt(CurrentScore / 1000.f);
	
	CoinsEarnedFromLastMatch = CoinMultiplier * (FMath::Floor((CurrentScore-LastScore) / 2) + ((HundredsCount-LastHundredsCount) * 10) + ((ThousandsCount-LastThousandsCount) * 100));
	CurrentCoinCount += CoinsEarnedFromLastMatch;

	LastScore = CurrentScore;
	CurrentScore = 0;
	
	// Could Play Sound
}

bool APlayerPaddle::OnPaddleBought(FName PaddleName)
{
	if(PaddleUnlockStatuses.Contains(PaddleName))
	{
		FPaddleInfo* PaddleInfoFound = PaddleDataTable->FindRow<FPaddleInfo>(PaddleName, TEXT(""), true);
		if(PaddleInfoFound != nullptr)
		{
			switch (PaddleInfoFound->PaddleRarity)
			{
			case EPaddleRarity::Common:
				if(CurrentCoinCount >= 200)
				{
					CurrentCoinCount -= 200;
					MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
					PaddleUnlockStatuses[PaddleName] = true;
                    for (int i = 0; i < 3; i++)
                    {
                        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
                    }
					
					return true;
				}
				break;
			case EPaddleRarity::Rare:
				if(CurrentCoinCount >= 400)
				{
					CurrentCoinCount -= 400;
					MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
					PaddleUnlockStatuses[PaddleName] = true;
                    for (int i = 0; i < 3; i++)
                    {
                        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
                    }
					return true;
				}
				break;
			case EPaddleRarity::Epic:
				if(CurrentCoinCount >= 800)
				{
					CurrentCoinCount -= 800;
					MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
					PaddleUnlockStatuses[PaddleName] = true;
                    for (int i = 0; i < 3; i++)
                    {
                        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
                    }
					return true;
				}
				break;
			case EPaddleRarity::Legendary:
				if(CurrentCoinCount >= 1500)
				{
					CurrentCoinCount -= 1500;
					MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
					PaddleUnlockStatuses[PaddleName] = true;
                    for (int i = 0; i < 3; i++)
                    {
                        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
                    }
					return true;
				}
				break;
			case EPaddleRarity::Mythic:
				if(CurrentCoinCount >= 2000)
				{
					CurrentCoinCount -= 2000;
					MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
					PaddleUnlockStatuses[PaddleName] = true;
                    for (int i = 0; i < 3; i++)
                    {
                        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
                    }
					return true;
				}
				break;
			default:
				break;
			}
		}
	}
	return false;
}

void APlayerPaddle::OnPaddleSelected(FName PaddleName)
{
	FPaddleInfo* PaddleInfoFound = PaddleDataTable->FindRow<FPaddleInfo>(PaddleName, TEXT(""), true);
	
	if(PaddleInfoFound != nullptr)
	{
		
		PaddleSprite->SetSprite(PaddleInfoFound->PaddleSprite);
		
		// Set Paddle Sound Effect
		if(PaddleInfoFound->PaddleSoundEffect != nullptr)
		{
			PaddleSoundEffect = PaddleInfoFound->PaddleSoundEffect;
		}
		
		if(PaddleInfoFound->PaddleAbility == EPaddleAbility::CoinMultiplier)
		{
			CoinMultiplier = 2;
		}
		else
		{
			CoinMultiplier = 1;
		}

		SetCurrentPaddle(PaddleName);
	}
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

void APlayerPaddle::OnGameLoaded()
{
    // Load Player Data
    if (PickleBallGameInstance)
    {
        FPlayerData PlayerData = PickleBallGameInstance->GetSaveGamePlayerData();

        LastScore = PlayerData.PlayersLastScore;
        CurrentScore = LastScore;
        CurrentCoinCount = PlayerData.PlayerCoins;
        HighScore = PlayerData.PlayerHighScore;
        PaddleUnlockStatuses = PlayerData.PaddleUnlockStatuses;
        CurrrentPaddleName = PlayerData.CurrentPaddleName;
    }
    OnPaddleSelected(CurrrentPaddleName);
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

FPlayerData APlayerPaddle::GetCurrentPlayerData()
{
	return FPlayerData(CurrentCoinCount, HighScore, CurrentScore, PaddleUnlockStatuses, CurrrentPaddleName);
}

int32 APlayerPaddle::GetHighScore() const
{
	return HighScore;
}

int32 APlayerPaddle::GetPlayerCoins() const
{
	return CurrentCoinCount;
}

int32 APlayerPaddle::GetCoinsEarnedFromLastMatch() const
{
	return CoinsEarnedFromLastMatch;
}

TMap<FName, bool> APlayerPaddle::GetPaddleUnlockStatuses() const
{
	return PaddleUnlockStatuses;
}

void APlayerPaddle::AddCoins(int32 CoinsToAdd)
{
	CurrentCoinCount += CoinsToAdd;
    for (int i = 0; i < 3; i++)
    {
        PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
    }
    
    MainGamemode->OnCoinAmountChanged.Broadcast(CurrentCoinCount);
}

int32 APlayerPaddle::GetCurrentScore() const
{
	return CurrentScore;
}

int32 APlayerPaddle::GetLastScore() const
{
	return LastScore;
}

void APlayerPaddle::SetCurrentPaddle(FName CurrrentPaddleNameIn)
{
	CurrrentPaddleName = CurrrentPaddleNameIn;
	if(PickleBallGameInstance != nullptr)
	{
        for (int i = 0; i < 3; i++)
        {
            PickleBallGameInstance->SavePlayerData(GetCurrentPlayerData());
        }
	}
}

void APlayerPaddle::SaveLastScore()
{
	if(PickleBallGameInstance != nullptr)
	{
		PickleBallGameInstance->SavePlayerData(FPlayerData(CurrentCoinCount, HighScore, LastScore, PaddleUnlockStatuses, CurrrentPaddleName));
	}
}

