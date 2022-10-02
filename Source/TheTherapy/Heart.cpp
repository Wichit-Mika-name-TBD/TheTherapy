// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "Heart.h"
#include "TheTherapyCharacter.h"
#include <QofL/log.h>
#include <QofL/obj_finder.h>

AHeart::AHeart()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_Heart"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->SetCanEverAffectNavigation(false);
}

auto AHeart::BeginPlay() -> void
{
  Super::BeginPlay();
  OnActorHit.AddDynamic(this, &AHeart::onHit);
};

auto AHeart::onHit(AActor *self, AActor *other, FVector imp, const FHitResult &hit) -> void
{
  LOG("on hit");
  auto character = Cast<ATheTherapyCharacter>(other);
  if (!character)
    return;
  character->addHeart();
  Destroy();
}
