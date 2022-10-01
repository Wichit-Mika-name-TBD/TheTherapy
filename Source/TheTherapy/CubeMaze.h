// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <array>

#include "CubeMaze.generated.h"

UCLASS()
class THETHERAPY_API ACubeMaze : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ACubeMaze();

  UPROPERTY(EditAnywhere)
  class USceneComponent *root;

  UPROPERTY(EditAnywhere)
  int sz = 8;

private:
  auto BeginPlay() -> void final;
  auto OnConstruction(const FTransform &Transform) -> void final;
  auto Tick(float DeltaTime) -> void final;

  FTransform initialTransform;

  std::array<class UBoxComponent *, 6> exitColliders;
};
