// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "MobSpawner.h"
#include "Mob.h"
#include <QofL/abbr.h>
#include <QofL/check_ret.h>
#include <QofL/class_finder.h>
#include <QofL/obj_finder.h>

AMobSpawner::AMobSpawner() : mobClass(CLASS_FINDER(AMob, "BP", "BP_Mob"))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_Exit"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->SetCanEverAffectNavigation(false);
  PrimaryActorTick.bCanEverTick = true;
}

auto AMobSpawner::spawn() -> void
{
  spawnTime = GetWorld()->GetTimeSeconds() + 20.f;
  LOG("requested to spawn mob");
}

auto AMobSpawner::despawn() -> void
{
  if (mob)
  {
    LOG("despawning mob");
    mob->Destroy();
  }
  mob = nullptr;
}

auto AMobSpawner::BeginPlay() -> void
{
  Super::BeginPlay();
  despawn();
  spawnTime = -1.f;
}

auto AMobSpawner::Tick(float dt) -> void
{
  Super::Tick(dt);

  if (spawnTime > GetWorld()->GetTimeSeconds() || spawnTime < 0)
    return;

  LOG("spawning mob");

  spawnTime = -1.f;

  CHECK_RET(mobClass);
  despawn();

  FActorSpawnParameters param;
  param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  mob = GetWorld()->SpawnActor<AMob>(mobClass, getLoc(this) + vec(0., 0., 100.), getRot(this), param);
  mob->SpawnDefaultController();
}
