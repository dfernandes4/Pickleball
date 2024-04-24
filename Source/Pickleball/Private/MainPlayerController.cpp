#include "MainPlayerController.h"
#include "GameFramework/Pawn.h"

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
