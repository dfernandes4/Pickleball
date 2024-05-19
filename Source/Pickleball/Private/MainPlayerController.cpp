#include "MainPlayerController.h"

#include "MainGamemode.h"
#include "OnlineSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Interfaces/OnlineIdentityInterface.h"


AMainPlayerController::AMainPlayerController()
{
    bIsFirstTouch = true;
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Check if InputComponent is valid and bind touch events
    if (InputComponent)
    {
        InputComponent->BindTouch(IE_Pressed, this, &AMainPlayerController::OnTouchPressed);
        InputComponent->BindTouch(IE_Released, this, &AMainPlayerController::OnTouchReleased);
        InputComponent->BindTouch(IE_Repeat, this, &AMainPlayerController::OnTouchMoved);
    }
}

void AMainPlayerController::BeginPlay()
{ 
    Super::BeginPlay();

    AMainGamemode* MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());
    if (MainGamemode != nullptr)
    {
        MainGamemode->OnGameOver.AddDynamic(this, &AMainPlayerController::OnGameOver);
    }

    DisableInput(this);
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bIsTouching && GetPawn())
    {
        FVector WorldLocation, WorldDirection;
        if (DeprojectScreenPositionToWorld(TouchLocation.X, TouchLocation.Y, WorldLocation, WorldDirection))
        {
            NewPaddleLocation = WorldLocation + TouchOffset;
            NewPaddleLocation.X = FMath::Clamp(NewPaddleLocation.X, -750.f,-88.f);
            FHitResult HitResult;
            GetPawn()->SetActorLocation(NewPaddleLocation, true, &HitResult, ETeleportType::TeleportPhysics);
            PaddleVelocity = (NewPaddleLocation - PaddleStartLocation) / DeltaTime;
            PaddleStartLocation = NewPaddleLocation; // Update start location for the next frame
        }
    }
}

void AMainPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    if (bIsFirstTouch)
    {
        OnFirstTouch.Broadcast();
        bIsFirstTouch = false;
    }
    
    bIsTouching = true;
    InitialTouchLocation = FVector2D(Location);
    TouchLocation = InitialTouchLocation;

    // Calculate the initial offset from the touch location to the paddle's location
    FVector WorldLocation, WorldDirection;
    if (DeprojectScreenPositionToWorld(InitialTouchLocation.X, InitialTouchLocation.Y, WorldLocation, WorldDirection))
    {
        TouchOffset = GetPawn()->GetActorLocation() - WorldLocation;
        PaddleStartLocation = GetPawn()->GetActorLocation();
    }
}

void AMainPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    bIsTouching = false;
}

void AMainPlayerController::OnTouchMoved(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    TouchLocation = FVector2D(Location);
}

FVector AMainPlayerController::GetPaddleVelocity() const
{
    return PaddleVelocity;
}

 void AMainPlayerController::InitiatePurchaseRequest(const FString& ProductId)
{

    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineStoreV2Ptr StoreInterface = OnlineSub->GetStoreV2Interface();
        if (StoreInterface.IsValid())
        {
            
            // Get the identity interface to obtain the user ID
            IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
            if (!IdentityInterface.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("Identity interface not available"));
                return;
            }

            // Obtain the user ID
            FUniqueNetIdPtr UserId = IdentityInterface->GetUniquePlayerId(0); // Assuming the first local player
            if (!UserId.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to get valid user ID"));
                return;
            }

            TArray<FUniqueOfferId> OfferIds;
            OfferIds.Add(FUniqueOfferId(ProductId));
            FOnQueryOnlineStoreOffersComplete OnQueryCompleteDelegate = FOnQueryOnlineStoreOffersComplete::CreateUObject(this, &AMainPlayerController::HandlePurchaseCompletion);
            // Trigger the purchase
            StoreInterface->QueryOffersById(*UserId, OfferIds, OnQueryCompleteDelegate);
        }
    }
}

void AMainPlayerController::HandlePurchaseCompletion(bool bWasSuccessful, const TArray<FUniqueOfferId>& Offers, const FString& ErrorMsg)
{
    if (bWasSuccessful && Offers.Num() > 0)
    {
        // Reward the player for that purchase
        const FString& OfferId = Offers[0];
        int32 CoinsAmount = 0;
        if(OfferId == "")
        {
            CoinsAmount = 100;
        }
        else if(OfferId == "")
        {
            CoinsAmount = 500;
        }
        else if(OfferId == "")
        {
            CoinsAmount = 1500;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Offer Id is not valid."));
            return;
        }
        OnPurchaseCompleted.Broadcast(CoinsAmount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Purchase failed: %s"), *ErrorMsg);
    }
}

void AMainPlayerController::OnGameOver()
{
    DisableInput(this);
}
