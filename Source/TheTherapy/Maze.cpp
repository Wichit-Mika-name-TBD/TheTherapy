// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "Maze.h"
#include "maze_gen.h"
#include <Components/InstancedStaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <QofL/abbr.h>
#include <QofL/obj_finder.h>

// Sets default values
AMaze::AMaze()
  : root(CreateDefaultSubobject<USceneComponent>("root")),
    cornerShort(CreateDefaultSubobject<UInstancedStaticMeshComponent>("cornerShort")),
    cornerTall(CreateDefaultSubobject<UInstancedStaticMeshComponent>("cornerTall")),
    wallShort(CreateDefaultSubobject<UInstancedStaticMeshComponent>("wallShort")),
    wallTall(CreateDefaultSubobject<UInstancedStaticMeshComponent>("wallTall"))
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;
  SetRootComponent(root);
  root->SetMobility(EComponentMobility::Movable);
  cornerShort->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
  cornerShort->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_CornerShort"));
  cornerTall->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
  cornerTall->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_CornerTall"));
  wallShort->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
  wallShort->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_WallShort"));
  wallTall->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
  wallTall->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_WallTall"));
}

// Called when the game starts or when spawned
auto AMaze::BeginPlay() -> void
{
  Super::BeginPlay();
}

// Called every frame
auto AMaze::Tick(float DeltaTime) -> void
{
  Super::Tick(DeltaTime);
}

auto AMaze::OnConstruction(const FTransform &Transform) -> void
{
  Super::OnConstruction(Transform);
  wallTall->ClearInstances();
  wallShort->ClearInstances();
  cornerTall->ClearInstances();
  cornerShort->ClearInstances();
  const auto sz = size();
  auto maze = genMaze(sz);
  for (auto y = 0; y < 2 * sz - 1; ++y)
    for (auto x = 0; x < sz - (y + 1) % 2; ++x)
    {
      if (!maze[x + y * sz])
        continue;
      if (y % 2 == 0)
      {
        if (rand() % 2 == 0)
          wallTall->AddInstance(FTransform(rot(0., 90., 0.), vec(x * 120., y / 2 * 120., 0.)));
        else
          wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(x * 120., y / 2 * 120., 0.)));
      }
      else
      {
        if (rand() % 2 == 0)
          wallTall->AddInstance(
            FTransform(rot(0., 0., 0.), vec(x * 120. + 10. - 120., y / 2 * 120. + 120. - 10., 0.)));
        else
          wallShort->AddInstance(
            FTransform(rot(0., 0., 0.), vec(x * 120. + 10. - 120., y / 2 * 120. + 120. - 10., 0.)));
      }
    }
  for (auto y = 0; y < sz + 1; ++y)
    for (auto x = 0; x < sz + 1; ++x)
    {
      if (rand() % 2 == 0)
        cornerTall->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120., y * 120. - 10., 0.)));
      else
        cornerShort->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120., y * 120. - 10., 0.)));
    }
}

auto AMaze::size() const -> int
{
  return 8;
}
