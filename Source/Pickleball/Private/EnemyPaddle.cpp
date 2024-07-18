// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPaddle.h"
#include "NiagaraComponent.h"
#include "Ball.h"
#include "EnemyAIController.h"
#include "EnemyAttributes.h"
#include "MainGamemode.h"
#include "PaperSpriteComponent.h"
#include "PickleBallGameInstance.h"
#include "PlayerPaddle.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "BackgroundFloor.h"


// Sets default values
AEnemyPaddle::AEnemyPaddle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	//Ramp up Movement based on enemies hits
	MovementComponent->Acceleration = 600;
	MovementComponent->Deceleration = 600;
	MovementComponent->MaxSpeed = 600.f;

	SwingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hiteffect"));
	SwingEffect->SetupAttachment(SceneComponent);
	

	ForceMultiplier = 1.5;
	bIsEnemiesTurn = true;
	CurrentRow = 10;
}

void AEnemyPaddle::BeginPlay()
{
	Super::BeginPlay();

	AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	if(MainGamemode)
	{
		MainGamemode->OnScoreUpdated.AddDynamic(this, &AEnemyPaddle::IncrementForceMultiplier);
		MainGamemode->OnGameOver.AddDynamic(this, &AEnemyPaddle::StopHitting);
	}

	PickleBallGameInstance = Cast<UPickleBallGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(PickleBallGameInstance != nullptr)
    {
        if(PickleBallGameInstance->GetIsGameLoaded())
        {
            OnGameLoaded();
        }
        else
        {
            PickleBallGameInstance->LoadFinished.AddDynamic(this, &AEnemyPaddle::OnGameLoaded);
        }
    }
}

void AEnemyPaddle::OnGameLoaded()
{
	CurrentRow = PickleBallGameInstance->GetSaveGameEnemyRow();
	int32 PlayersLastScore = PickleBallGameInstance->GetSaveGamePlayerData().PlayersLastScore;
	if(PlayersLastScore > 0 && PlayersLastScore <= 20)
	{
		ForceMultiplier += PlayersLastScore / 10;
	}
	
	if(!PickleBallGameInstance->GetIsFirstTimePlaying())
	{
		SetRandomEnemyAttributes();
	}

	PickleBallGameInstance->LoadFinished.RemoveDynamic(this, &AEnemyPaddle::OnGameLoaded);
}

void AEnemyPaddle::HitBall()
{
	FlipPaddle();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PaddleSoundEffect, GetActorLocation());
	//Might change values, ENEMY CANNOT MISS!
	if(bIsFirstSwing)
	{
		BallInScene->BallMesh->SetEnableGravity(true);
		bIsFirstSwing = false;
		
		BallInScene->ApplySwipeForce(FVector(-42,13.5,35), this);
		
	}
	else
	{
		FVector RandomForce;
		const int32 RandNum = FMath::RandRange(1, 5);
		
		constexpr float YOuterBounds = 372.f;
		constexpr float FarthestHittingLocation = 895.0f;
		
		const float PercentageOfYDistanceFromCenter = GetActorLocation().Y / YOuterBounds;
		const float PercentageOfXDistanceFromFarthestHittingLocation = FMath::Clamp(GetActorLocation().X / FarthestHittingLocation, 0,1);
		
		// Based on position from center
        float MinYVal = 0;
		float MaxYVal = 0;
		
		if(RandNum != 1)
		{
            MinYVal = -20.f;
            MaxYVal = 20.f;
            
			// Max -176 Min -48
			RandomForce.X = (-32 * ForceMultiplier);
			
			// Interpolation for Z
			const float MinZ =  (1.854 * (FMath::Pow(10.f, -5.f) * FMath::Pow(RandomForce.X, 3.f))) +
								(7.416 * (FMath::Pow(10.f, -3.f) * FMath::Pow(RandomForce.X, 2.f))) + (1.092 * RandomForce.X) + 54.61;
		
			const float MaxZ =  (7.065 * (FMath::Pow(10.f, -13.f) * FMath::Pow(RandomForce.X, 7.f))) + 
								(5.7551 * (FMath::Pow(10.f, -10.f) * FMath::Pow(RandomForce.X, 6.f))) + 
								(1.9777 * (FMath::Pow(10.f, -7.f) * FMath::Pow(RandomForce.X, 5.f))) + 
								(.0000373143f * FMath::Pow(RandomForce.X, 4.f)) + 
								(.00421209 * FMath::Pow(RandomForce.X, 3.f)) + 
								(0.290215 * FMath::Pow(RandomForce.X, 2.f)) + (11.9063 * RandomForce.X) + 262.738;

			RandomForce.Z = MinZ + ((MaxZ-MinZ) * PercentageOfXDistanceFromFarthestHittingLocation);
		}
		else
		{
            MinYVal = -5.f;
            MaxYVal = 5.f;
            
			//Occasional kitchen hits
			RandomForce.X = -12 + (-25 * PercentageOfXDistanceFromFarthestHittingLocation);
			RandomForce.Z = 26 + (13 * PercentageOfXDistanceFromFarthestHittingLocation);
		}
        
        RandomForce.Y = FMath::RandRange(MinYVal * (.5 + PercentageOfYDistanceFromCenter), MaxYVal * (.5 - PercentageOfYDistanceFromCenter));
        
		// Apply force
		BallInScene->ApplySwipeForce(RandomForce, this);
	}

	bIsEnemiesTurn = false;
	Cast<AEnemyAIController>(GetController())->SetIdleState();
}

void AEnemyPaddle::FlipPaddle()
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
		SwingEffectCurrentRotation.Yaw += 30.0;
	}
	else
	{
		bIsFacingLeft = false;
		CurrentRotation.Yaw += 90.0f;
		SwingEffectCurrentRotation.Pitch -=180.0f;
		SwingEffectCurrentLocation.Y += 48.0;
		SwingEffectCurrentRotation.Yaw  -= 30.0;
	}
	SwingEffect->ResetSystem();
		
	this->PaddleSprite->SetRelativeRotation(CurrentRotation,false, nullptr, ETeleportType::TeleportPhysics);
	this->SwingEffect->SetRelativeRotation(SwingEffectCurrentRotation,false,nullptr,ETeleportType::TeleportPhysics);

	this->SwingEffect->SetRelativeLocation(SwingEffectCurrentLocation,false,nullptr,ETeleportType::TeleportPhysics);

}

void AEnemyPaddle::IncrementForceMultiplier(int NewScore)
{
	if (ForceMultiplier <= 3.5)
	{
		ForceMultiplier += .1;
	}
	else
	{
		AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

		if(MainGamemode)
		{
			MainGamemode->OnScoreUpdated.RemoveDynamic(this, &AEnemyPaddle::IncrementForceMultiplier);
		}
	}
}

void AEnemyPaddle::SetRandomEnemyAttributes()
{
    int32 RandomNumber = FMath::RandRange(1, 10000);

    if (CurrentRow == 0)
    {
        if (RandomNumber != 10000)
        {
            CurrentRow = FMath::RandRange(1, 15);
        }
        else
        {
            CurrentRow = 16;
        }
    }

    FName CurrentRowName = FName(*FString::Printf(TEXT("%d"), CurrentRow)); // Convert index to FName

    if (EnemyAttributesDataTable != nullptr)
    {
        CurrentEnemyAttributes = EnemyAttributesDataTable->FindRow<FEnemyAttributes>(CurrentRowName, TEXT("LookupEnemyAttributes"));
        if (CurrentEnemyAttributes)
        {
            // Set sprite and sound effect
            PaddleSprite->SetSprite(CurrentEnemyAttributes->PaddleSprite);
            PaddleSoundEffect = CurrentEnemyAttributes->PaddleSoundEffect;

            // Load material asynchronously
            if (CurrentEnemyAttributes->BackgroundMaterial)
            {
                FSoftObjectPath MaterialPath = CurrentEnemyAttributes->BackgroundMaterial->GetPathName();
                if (MaterialPath.IsValid())
                {
                    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
                    Streamable.RequestAsyncLoad(MaterialPath, FStreamableDelegate::CreateUObject(this, &AEnemyPaddle::OnMaterialLoaded, CurrentRowName));
                }
            }
        }
    }
}

void AEnemyPaddle::OnMaterialLoaded(FName RowName)
{
    UE_LOG(LogTemp, Warning, TEXT("Material Has Been Loaded"));
    CurrentEnemyAttributes = EnemyAttributesDataTable->FindRow<FEnemyAttributes>(RowName, TEXT("OnMaterialLoaded"));
    if (CurrentEnemyAttributes && CurrentEnemyAttributes->BackgroundMaterial)
    {
        UMaterial* LoadedMaterial = CurrentEnemyAttributes->BackgroundMaterial;
        
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Background"), FoundActors);

        if (FoundActors.Num() > 0)
        {
            BackgroundFloor = Cast<ABackgroundFloor>(FoundActors[0]);
            if(BackgroundFloor != nullptr)
            {
                BackgroundFloor->SetNewMaterial(LoadedMaterial);
            }
        }
    }
}

void AEnemyPaddle::SetIsEnemiesTurn(bool bIsTurn)
{
	bIsEnemiesTurn = bIsTurn;
}

bool AEnemyPaddle::GetIsEnemiesTurn() const
{
    return bIsEnemiesTurn;
}

void AEnemyPaddle::SetCurrentRow(int32 Row)
{
	CurrentRow = Row;
}

int32 AEnemyPaddle::GetCurrentRow() const
{
	return CurrentRow;
}

void AEnemyPaddle::OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPaddleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Only hit once and don't if player lost
	if(bIsEnemiesTurn && OtherActor->IsA(ABall::StaticClass()))
	{
		HitBall();
	}
}

void AEnemyPaddle::StopHitting()
{
	bIsEnemiesTurn = false;
	Cast<AEnemyAIController>(GetController())->SetIdleState();
}

void AEnemyPaddle::AdjustEnemySpeed(const FVector& BallVelocity, const FVector& HittingLocation)
{
	FVector EnemyLocation = GetActorLocation();
	float DistanceToTarget = FVector::Dist(EnemyLocation, HittingLocation);

	
	float BallSpeed = BallVelocity.Size();
	float TimeToTarget = DistanceToTarget / BallSpeed;
	
	float RequiredSpeed = DistanceToTarget / TimeToTarget;
	MovementComponent->Acceleration = RequiredSpeed / TimeToTarget;
	MovementComponent->Deceleration = RequiredSpeed / TimeToTarget;
	
	MovementComponent->MaxSpeed = RequiredSpeed * 1.25;
}
