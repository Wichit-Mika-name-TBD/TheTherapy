// Copyright (c) 2022 Wichit & Mika (name TBD)

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Heart.generated.h"

UCLASS()
class THETHERAPY_API AHeart final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AHeart();

private:
  auto BeginPlay() -> void final;

  UFUNCTION()
  void onHit(AActor *self, AActor *other, FVector imp, const FHitResult &hit);
};
