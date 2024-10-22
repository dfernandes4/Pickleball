// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGamemode.h"

#include "CollectionWidget.h"
#include "EnemyAIController.h"
#include "HomeScreenWidget.h"
#include "LoadingScreenWidget.h"
#include "PickleBallGameInstance.h"
#include "ShopScreenWidget.h"
#include "Sound/SoundClass.h"
#include "UserWidgetLoader.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AMainGamemode::AMainGamemode()
{
	OnGameOver.AddDynamic(this, &AMainGamemode::GameOver);
	OnCountdownKickoffFinished.AddDynamic(this, &AMainGamemode::OnGameStart);

    Scene = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
    SetRootComponent(Scene);
    
	BattleMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleMusic"));
	BattleMusic->SetupAttachment(Scene);
    
    HomeScreenMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("HomeScreenMusic"));
    HomeScreenMusic->SetupAttachment(Scene);

	bIsGameActive = false;
	bKickOffFuncCalled = false;
}

void AMainGamemode::BeginPlay()
{
	Super::BeginPlay();

	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	
	PickleBallGameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if(PickleBallGameInstance->GetShouldLaunchStarterScreen())
	{
		if(!PickleBallGameInstance->GetIsFirstTimePlayingInSession())
		{
			// 2nd+ time playing in session
			
			WidgetLoader->LoadWidget(FName("LoadingScreen"), GetWorld(), 10);
			UHomeScreenWidget* HomeScreenWidget = Cast<UHomeScreenWidget>(WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld()));
			HomeScreenWidget->CollectionWidget	= Cast<UCollectionWidget>(WidgetLoader->LoadWidget(FName("CollectionScreen"), GetWorld()));
			HomeScreenWidget->DisplayBasePaddles();
			HomeScreenWidget->SetUpCollectionDelegates();
			HomeScreenWidget->ShopWidget		= Cast<UShopScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleShopScreen"), GetWorld()));
			HomeScreenWidget->SetUpShopDelegates();
		}
		else
		{

            // First time playing in session
            WidgetLoader->LoadWidget(FName("LoadingScreen"), GetWorld(), 10);
            if(PickleBallGameInstance->GetIsGameLoaded())
            {
                OnGameLoaded();
            }
            else
            {
                PickleBallGameInstance->LoadFinished.AddDynamic(this, &AMainGamemode::OnGameLoaded);
            }
		}
		
		PlayerController->DisableInput(PlayerController);
	}
	else
	{
		// 2nd+ time playing in session
		if(PickleBallGameInstance->GetIsContinueGame())
		{
			PickleBallGameInstance->SetIsContinueGame(false);
			WidgetLoader->LoadWidget(FName("PlayButton"), GetWorld(), 15);
		}
		else
		{
			ULoadingScreenWidget* LoadingScreen = Cast<ULoadingScreenWidget>(WidgetLoader->LoadWidget(FName("LoadingScreen"), GetWorld(), 10));
			LoadingScreen->LoadingScreenFinished.AddDynamic(this, &AMainGamemode::OnLoadingScreenFinished);
		}
	} 
	CachedEnemyAIController = Cast<AEnemyAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyAIController::StaticClass()));
}

void AMainGamemode::OnGameLoaded()
{
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	
	if(PickleBallGameInstance->GetIsFirstTimePlaying())
	{
		WidgetLoader->LoadWidget(FName("TutorialScreen"), GetWorld());
	}
	else
	{
		UHomeScreenWidget* HomeScreenWidget = Cast<UHomeScreenWidget>(WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld()));
		HomeScreenWidget->CollectionWidget	= Cast<UCollectionWidget>(WidgetLoader->LoadWidget(FName("CollectionScreen"), GetWorld()));
		HomeScreenWidget->DisplayBasePaddles();
		HomeScreenWidget->SetUpCollectionDelegates();
		HomeScreenWidget->ShopWidget		= Cast<UShopScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleShopScreen"), GetWorld()));
		HomeScreenWidget->SetUpShopDelegates();
	}

	TArray<float> Volumes = PickleBallGameInstance->GetSaveGameVolumes();
	if(MasterSoundClass != nullptr)
	{
		MasterSoundClass->Properties.Volume = Volumes[0];
	}
	if(MusicSoundClass != nullptr)
	{
		MusicSoundClass->Properties.Volume = Volumes[1];
	}
	if(SFXSoundClass != nullptr)
	{
		SFXSoundClass->Properties.Volume = Volumes[2];
	}
	
	PickleBallGameInstance->SetIsFirstTimePlayingInSession(false);
	PickleBallGameInstance->LoadFinished.RemoveDynamic(this, &AMainGamemode::OnGameLoaded);
}

void AMainGamemode::OnLoadingScreenFinished()
{
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
}

void AMainGamemode::CountdownTimerFinished()
{
	UGameplayStatics::PlaySound2D(GetWorld(), CountDownSoundKickoffEffect);
	float CountDownKickOffEffectDuration = CountDownSoundKickoffEffect->Duration;
	FTimerHandle KickOffTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(KickOffTimerHandle, this, &AMainGamemode::KickOffFinished, CountDownKickOffEffectDuration, false);
	// Need to check if KickOff Isn't called after 1 second of the end of the kickoff duration

	FTimerHandle CheckingTimer;
	GetWorld()->GetTimerManager().SetTimer(CheckingTimer, this, &AMainGamemode::KickOffFinished, (CountDownKickOffEffectDuration + 1.f), false);
}

void AMainGamemode::KickOffFinished()
{
	if(!bKickOffFuncCalled)
	{
		bKickOffFuncCalled = true;
		if(CachedEnemyAIController != nullptr)
		{
			CachedEnemyAIController->StartBehaviorTree();
		}
		OnGameStart();
	}
}

void AMainGamemode::GameOver()
{
	BattleMusic->Stop();
	
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
	if(GameInstance->GetIsFirstTimePlaying())
	{
		GameInstance->SetIsFirstTimePlayingEver(false);
	}
	OnGameOver.RemoveDynamic(this, &AMainGamemode::GameOver);
	
	bIsGameActive = false;
}

void AMainGamemode::OnGameStart()
{
	BattleMusic->Play();
	bIsGameActive = true;
}
