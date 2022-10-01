// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze.generated.h"

UCLASS()
class THETHERAPY_API AMaze : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AMaze();

protected:
  virtual auto size() const -> int;

private:
  // Called when the game starts or when spawned
  auto BeginPlay() -> void final;
  auto OnConstruction(const FTransform &Transform) -> void final;

  // Called every frame
  auto Tick(float DeltaTime) -> void final;

  class USceneComponent *root;
  class UInstancedStaticMeshComponent *cornerShort;
  class UInstancedStaticMeshComponent *cornerTall;
  class UInstancedStaticMeshComponent *wallShort;
  class UInstancedStaticMeshComponent *wallTall;
};
