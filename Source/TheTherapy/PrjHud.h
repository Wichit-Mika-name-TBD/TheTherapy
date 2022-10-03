// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PrjHud.generated.h"

UCLASS()
class THETHERAPY_API APrjHud final : public AHUD
{
  GENERATED_BODY()
public:
  APrjHud();
  class UHudUi *hudUi = nullptr;

private:
  auto BeginPlay() -> void final;

};
