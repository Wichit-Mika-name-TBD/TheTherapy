// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "MobController.generated.h"

UCLASS()
class THETHERAPY_API AMobController final : public AAIController
{
  GENERATED_BODY()
public:
  AMobController();

private:
  auto Tick(float) -> void final;
};
