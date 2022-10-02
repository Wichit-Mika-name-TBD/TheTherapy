// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "Mob.h"
#include <AIController.h>
#include <Components/CapsuleComponent.h>
#include <QofL/class_finder.h>

// Sets default values
AMob::AMob()
{
  GetCapsuleComponent()->InitCapsuleSize(21.f, 96.0f);

  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;
  AIControllerClass = CLASS_FINDER(AAIController, "BP", "BP_MobController");
}

// Called when the game starts or when spawned
void AMob::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AMob::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMob::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}
