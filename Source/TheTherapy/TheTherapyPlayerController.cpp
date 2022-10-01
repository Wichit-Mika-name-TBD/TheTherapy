// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheTherapyPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TheTherapyCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <QofL/abbr.h>
#include <QofL/check_ret.h>
#include <QofL/log.h>

ATheTherapyPlayerController::ATheTherapyPlayerController()
{
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;
}

void ATheTherapyPlayerController::PlayerTick(float DeltaTime)
{
  Super::PlayerTick(DeltaTime);

  if (bInputPressed)
  {
    FollowTime += DeltaTime;

    // Look for the touch location
    FVector HitLocation = FVector::ZeroVector;
    FHitResult Hit;
    if (bIsTouch)
    {
      GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
    }
    else
    {
      GetHitResultUnderCursor(ECC_Visibility, true, Hit);
    }
    HitLocation = Hit.Location;

    // Direct the Pawn towards that location
    APawn *const MyPawn = GetPawn();
    if (MyPawn)
    {
      FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
      MyPawn->AddMovementInput(WorldDirection, 1.f, false);
    }
  }
  else
  {
    FollowTime = 0.f;
  }
}

void ATheTherapyPlayerController::SetupInputComponent()
{
  // set up gameplay key bindings
  Super::SetupInputComponent();

  InputComponent->BindAction(
    "SetDestination", IE_Pressed, this, &ATheTherapyPlayerController::OnSetDestinationPressed);
  InputComponent->BindAction(
    "SetDestination", IE_Released, this, &ATheTherapyPlayerController::OnSetDestinationReleased);

  // support touch devices
  InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATheTherapyPlayerController::OnTouchPressed);
  InputComponent->BindTouch(
    EInputEvent::IE_Released, this, &ATheTherapyPlayerController::OnTouchReleased);

  InputComponent->BindAxis("MouseMotion", this, &ATheTherapyPlayerController::OnMouseMotion);
  InputComponent->BindAction(
    "StartCameraOrbit", IE_Pressed, this, &ATheTherapyPlayerController::OnStartCameraOrbit);
  InputComponent->BindAction(
    "StartCameraOrbit", IE_Released, this, &ATheTherapyPlayerController::OnFinishCameraOrbit);
}

void ATheTherapyPlayerController::OnSetDestinationPressed()
{
  // We flag that the input is being pressed
  bInputPressed = true;
  // Just in case the character was moving because of a previous short press we stop it
  StopMovement();
}

void ATheTherapyPlayerController::OnSetDestinationReleased()
{
  // Player is no longer pressing the input
  bInputPressed = false;

  // If it was a short press
  if (FollowTime <= ShortPressThreshold)
  {
    // We look for the location in the world where the player has pressed the input
    FVector HitLocation = FVector::ZeroVector;
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, true, Hit);
    HitLocation = Hit.Location;

    // We move there and spawn some particles
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
                                                   FXCursor,
                                                   HitLocation,
                                                   FRotator::ZeroRotator,
                                                   FVector(1.f, 1.f, 1.f),
                                                   true,
                                                   true,
                                                   ENCPoolMethod::None,
                                                   true);
  }
}

void ATheTherapyPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex,
                                                 const FVector Location)
{
  bIsTouch = true;
  OnSetDestinationPressed();
}

void ATheTherapyPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex,
                                                  const FVector Location)
{
  bIsTouch = false;
  OnSetDestinationReleased();
}

auto ATheTherapyPlayerController::OnMouseMotion(float val) -> void
{
  if (val == 0)
    return;
  if (!isCameraOrbiting)
    return;
  auto character = Cast<ATheTherapyCharacter>(GetPawnOrSpectator());
  CHECK_RET(character);
  auto boom = character->GetCameraBoom();
  CHECK_RET(boom);
  const auto K = 4.;
  boom->AddRelativeRotation(rot(0., K * val, 0.));
}

auto ATheTherapyPlayerController::OnStartCameraOrbit() -> void
{
  LOG("Start Camera Orbit");
  isCameraOrbiting = true;
}

auto ATheTherapyPlayerController::OnFinishCameraOrbit() -> void
{
  LOG("Finish Camera Orbit");
  isCameraOrbiting = false;
}
