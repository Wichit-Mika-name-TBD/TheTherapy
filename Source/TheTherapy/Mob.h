// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mob.generated.h"

UCLASS()
class THETHERAPY_API AMob final : public ACharacter
{
  GENERATED_BODY()

public:
  AMob();

  UFUNCTION(BlueprintCallable)
  bool isAttacking() const;

private:
  auto BeginPlay() -> void final;
  auto Tick(float DeltaTime) -> void final;
  auto SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) -> void final;
};
