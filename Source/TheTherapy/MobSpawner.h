// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "MobSpawner.generated.h"

UCLASS()
class THETHERAPY_API AMobSpawner final : public AStaticMeshActor
{
  GENERATED_BODY()

public:
  AMobSpawner();
  auto spawn() -> void;
  auto despawn() -> void;

private:
  auto BeginPlay() -> void final;
  auto Tick(float) -> void final;

  class AMob *mob = nullptr;
  TSubclassOf<class AMob> mobClass = nullptr;
  float spawnTime = -1.f;
};
