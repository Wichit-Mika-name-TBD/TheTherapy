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
    wallTall(CreateDefaultSubobject<UInstancedStaticMeshComponent>("wallTall")),
    floor(CreateDefaultSubobject<UInstancedStaticMeshComponent>("floor"))
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
  floor->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
  floor->SetStaticMesh(OBJ_FINDER(StaticMesh, "Mesh", "SM_Floor"));
}

// Called when the game starts or when spawned
auto AMaze::BeginPlay() -> void
{
  Super::BeginPlay();
  nextRegen = 10.f;
  regenMaze();
}

// Called every frame
auto AMaze::Tick(float DeltaTime) -> void
{
  Super::Tick(DeltaTime);
  if (GetWorld()->GetTimeSeconds() > nextRegen)
  {
    nextRegen += 10.f;
    regenMaze();
  }
}

auto AMaze::OnConstruction(const FTransform &Transform) -> void
{
  Super::OnConstruction(Transform);
  regenMaze();
}

auto AMaze::regenMaze() -> void
{
  wallTall->ClearInstances();
  wallShort->ClearInstances();
  cornerTall->ClearInstances();
  cornerShort->ClearInstances();
  floor->ClearInstances();
  auto maze = genMaze(sz, rmFraction);
  for (auto y = 0; y < 2 * sz - 1; ++y)
    for (auto x = 0; x < sz - (y + 1) % 2; ++x)
    {
      if (!maze[x + y * sz])
        continue;
      if (y % 2 == 0)
      {
        if (rand() % 2 == 0)
          wallTall->AddInstance(
            FTransform(rot(0., 90., 0.), vec(x * 120. + 120., y / 2 * 120. + 10., 0.)));
        else
          wallShort->AddInstance(
            FTransform(rot(0., 90., 0.), vec(x * 120. + 120., y / 2 * 120. + 10., 0.)));
      }
      else
      {
        if (rand() % 2 == 0)
          wallTall->AddInstance(
            FTransform(rot(0., 0., 0.), vec(x * 120. + 10., y / 2 * 120. + 120., 0.)));
        else
          wallShort->AddInstance(
            FTransform(rot(0., 0., 0.), vec(x * 120. + 10., y / 2 * 120. + 120., 0.)));
      }
    }
  for (auto y = 0; y < sz + 1; ++y)
    for (auto x = 0; x < sz + 1; ++x)
    {
      if (rand() % 2 == 0)
        cornerTall->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120., y * 120., 0.)));
      else
        cornerShort->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120., y * 120., 0.)));
    }
  for (auto y = -2; y < sz + 2; ++y)
    for (auto x = -2; x < sz + 2; ++x)
      floor->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120., y * 120. + 120. + 10., 0.)));

  for (auto x = 0; x < sz; ++x)
  {
    if (!((x == sz - 1 && (startCorner == 0 || endCorner == 0)) ||
          (x == 0 && (startCorner == 1 || endCorner == 1))))
      wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120. + 10., 0., 0.)));
    if (!((x == sz - 1 && (startCorner == 2 || endCorner == 2)) ||
          (x == 0 && (startCorner == 3 || endCorner == 3))))
      wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(x * 120. + 10., sz * 120., 0.)));
  }
  for (auto y = 0; y < sz; ++y)
  {
    if (!((y == sz - 1 && (startCorner == 4 || endCorner == 4)) ||
          (y == 0 && (startCorner == 5 || endCorner == 5))))
      wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., y * 120. + 10., 0.)));
    if (!((y == sz - 1 && (startCorner == 6 || endCorner == 6)) ||
          (y == 0 && (startCorner == 7 || endCorner == 7))))
      wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., y * 120. + 10., 0.)));
  }

  if (endCorner == 0)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., -2 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., -2 * 120. + 10., 0.)));
  }
  if (endCorner == 1)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., -2 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(120., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(120., -2 * 120. + 10., 0.)));
  }
  if (endCorner == 2)
  {
    wallShort->AddInstance(
      FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(
      FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., sz * 120. + 1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., sz * 120. + 1 * 120. + 10., 0.)));
  }
  if (endCorner == 3)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., sz * 120. + 1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(120., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(120., sz * 120. + 1 * 120. + 10., 0.)));
  }
  if (endCorner == 4)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., sz * 120. - 1 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-2 * 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-2 * 120., sz * 120. - 1 * 120., 0.)));
  }
  if (endCorner == 5)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., 0 * 120. + 1 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-2 * 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-2 * 120., 0 * 120. + 1 * 120., 0.)));
  }
  if (endCorner == 6)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., sz * 120. - 1 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120. + 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120. + 120., sz * 120. - 1 * 120., 0.)));
  }
  if (endCorner == 7)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., 0 * 120. + 1 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120. + 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120. + 120., 0 * 120. + 1 * 120., 0.)));
  }
  if (startCorner == 0)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., -2 * 120. + 10., 0.)));
  }
  if (startCorner == 1)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., -1 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., -2 * 120. + 10., 0.)));
  }

  if (startCorner == 2)
  {
    wallShort->AddInstance(
      FTransform(rot(0., 90., 0.), vec((sz - 1) * 120., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(sz * 120., sz * 120. + 0 * 120. + 10., 0.)));
  }
  if (startCorner == 3)
  {
    wallShort->AddInstance(FTransform(rot(0., 90., 0.), vec(0., sz * 120. + 0 * 120. + 10., 0.)));
    wallShort->AddInstance(
      FTransform(rot(0., 90., 0.), vec(2. * 120. - 120., sz * 120. + 0 * 120. + 10., 0.)));
  }
  if (startCorner == 4)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., sz * 120. - 1 * 120., 0.)));
  }
  if (startCorner == 5)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(-1 * 120., 0 * 120. + 1 * 120., 0.)));
  }
  if (startCorner == 6)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., sz * 120. - 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., sz * 120. - 1 * 120., 0.)));
  }
  if (startCorner == 7)
  {
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., 0 * 120. + 0 * 120., 0.)));
    wallShort->AddInstance(FTransform(rot(0., 0., 0.), vec(sz * 120., 0 * 120. + 1 * 120., 0.)));
  }
}
