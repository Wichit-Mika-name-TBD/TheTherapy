// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "HudUi.generated.h"

UCLASS()
class THETHERAPY_API UHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto setup() -> void;
  auto setHearts(int) -> void;
  auto setTime(float) -> void;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FName nextLevel;
};
