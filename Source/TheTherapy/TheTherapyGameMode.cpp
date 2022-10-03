// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheTherapyGameMode.h"
#include "PrjHud.h"
#include "TheTherapyCharacter.h"
#include "TheTherapyPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ATheTherapyGameMode::ATheTherapyGameMode()
{
  HUDClass = APrjHud::StaticClass();

  // use our custom PlayerController class
  PlayerControllerClass = ATheTherapyPlayerController::StaticClass();

  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/BP_TopDownCharacter"));
  if (PlayerPawnBPClass.Class != nullptr)
  {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }

  // set default controller to our Blueprinted controller
  static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
    TEXT("/Game/BP/BP_TopDownPlayerController"));
  if (PlayerControllerBPClass.Class != NULL)
  {
    PlayerControllerClass = PlayerControllerBPClass.Class;
  }
}
