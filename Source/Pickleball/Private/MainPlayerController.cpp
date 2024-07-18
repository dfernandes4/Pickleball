#include "MainPlayerController.h"

#include "MainGamemode.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "OnlineSubsystem.h"
#include "GameFramework/Pawn.h"
#include "OnlineError.h" 
#include "PickleBallGameInstance.h"


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
    LoginToGameCenter();
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
            NewPaddleLocation.X = FMath::Clamp(NewPaddleLocation.X, -844, -88.f);
            NewPaddleLocation.Y = FMath::Clamp(NewPaddleLocation.Y, -412, 412.f);
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
            FOnQueryOnlineStoreOffersComplete OnQueryCompleteDelegate = FOnQueryOnlineStoreOffersComplete::CreateUObject(this, &AMainPlayerController::OnQueryOffersComplete);
            // Trigger the offer query
            StoreInterface->QueryOffersById(*UserId, OfferIds, OnQueryCompleteDelegate);
        }
    }
}

void AMainPlayerController::OnQueryOffersComplete(bool bWasSuccessful, const TArray<FUniqueOfferId>& Offers, const FString& ErrorMsg)
{
    if (bWasSuccessful && Offers.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully retrieved offers."));
        
        IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
        if (OnlineSub)
        {
            IOnlineStoreV2Ptr StoreInterface = OnlineSub->GetStoreV2Interface();
            if (StoreInterface.IsValid())
            {
                TSharedPtr<FOnlineStoreOffer> OfferPtr = StoreInterface->GetOffer(Offers[0]);
                // Check if OfferPtr is valid
                if (OfferPtr.IsValid())
                {
                    // Convert TSharedPtr to TSharedRef
                    TSharedRef<FOnlineStoreOffer> OfferRef = OfferPtr.ToSharedRef();
                    PurchaseOffer(OfferRef);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to retrieve offers or no offers available."));
        }
    }
}

void AMainPlayerController::PurchaseOffer(FOnlineStoreOfferRef Offer)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlinePurchasePtr PurchaseInterface = OnlineSub->GetPurchaseInterface();
        if (PurchaseInterface.IsValid())
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

            bool bIsConsumable = false;
            if (Offer->DynamicFields.Contains("isConsumable"))
            {
                bIsConsumable = Offer->DynamicFields["isConsumable"].ToBool();
            }
            
            FPurchaseCheckoutRequest CheckoutRequest;
            CheckoutRequest.AddPurchaseOffer("", Offer->OfferId, 1, bIsConsumable);

            FOnPurchaseCheckoutComplete OnPurchaseCompleteDelegate = FOnPurchaseCheckoutComplete::CreateUObject(this, &AMainPlayerController::HandlePurchaseCompletion);
            // Trigger the purchase
            PurchaseInterface->Checkout(*UserId, CheckoutRequest, OnPurchaseCompleteDelegate);
        }
    }
}

void AMainPlayerController::HandlePurchaseCompletion(const FOnlineError& Result, const TSharedRef<FPurchaseReceipt>& PurchaseReceipt)
{
    if (Result.WasSuccessful())
    {
        const FString& ProductId = PurchaseReceipt.Get().ReceiptOffers[0].OfferId;
        UE_LOG(LogTemp, Log, TEXT("%s"), *ProductId);
        
        int32 CoinsAmount = 0;

        if(ProductId == "SomeGold")
        {
            CoinsAmount = 200;
        }
        else if(ProductId == "FistoGold")
        {
            CoinsAmount = 400;
        }
        else if(ProductId == "LotsoGold")
        {
            CoinsAmount = 650;
        }
        else if(ProductId == "PilesoGold")
        {
            CoinsAmount = 1500;
        }
        else if(ProductId == "RemoveAds")
        {
            UPickleBallGameInstance* PickleBallGameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
            if(PickleBallGameInstance != nullptr)
            {
                PickleBallGameInstance->RemoveAds();
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Product Id is not valid."));
            return;
        }

        OnPurchaseCompleted.Broadcast(CoinsAmount);
    }
}

void AMainPlayerController::LoginToGameCenter()
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
        if (Identity.IsValid())
        {
            FOnLoginCompleteDelegate Delegate = FOnLoginCompleteDelegate::CreateUObject(this, &AMainPlayerController::OnLoginComplete);
            Identity->AddOnLoginCompleteDelegate_Handle(0, Delegate);

            FOnlineAccountCredentials AccountCredentials;
            Identity->Login(0, AccountCredentials);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Identity interface is not valid"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem IOS is not available"));
    }
}

void AMainPlayerController::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
        if (Identity.IsValid())
        {
            Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, DelegateHandle);

            if (bWasSuccessful)
            {
                
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Login to Game Center failed: %s"), *Error);
                // Handle login failure
            }
        }
    }
}

void AMainPlayerController::ShowLeaderboard(FName CategoryName)
{
    if(IsLoggedInToGameCenter())
    {
        IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
        if (OnlineSub)
        {
            IOnlineExternalUIPtr ExternalUI = OnlineSub->GetExternalUIInterface();
            if (ExternalUI.IsValid())
            {
                ExternalUI->ShowLeaderboardUI(CategoryName.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("External UI interface is not valid"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is not available"));
        }
    }
    else
    {
        LoginToGameCenter();
    }
}

void AMainPlayerController::SubmitHighscore(int32 Score, FName CategoryName)
{
    if (IsLoggedInToGameCenter())
    {
        IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
        if (OnlineSub)
        {
            IOnlineLeaderboardsPtr LeaderboardsInterface = OnlineSub->GetLeaderboardsInterface();
            if (LeaderboardsInterface.IsValid())
            {
                // Create a leaderboard score object
                FOnlineLeaderboardWrite WriteObject;
                WriteObject.LeaderboardName = CategoryName.ToString();
                WriteObject.SetIntStat(TEXT("Score"), Score);
                
                // Submit the score to the leaderboard
                LeaderboardsInterface->WriteLeaderboardIntegerData(CategoryName, WriteObject, WriteCompleteDelegate);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Leaderboards interface is not valid"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is not available"));
        }
    }
    else
    {
        LoginToGameCenter(); // Ensure the player is logged in before submitting scores
    }
}

bool AMainPlayerController::IsLoggedInToGameCenter()
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName("IOS"));
    if (OnlineSub)
    {
        IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
        if (Identity.IsValid())
        {
            return Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn;
        }
    }
    return false;
}
void AMainPlayerController::OnGameOver()
{
    DisableInput(this);
}
