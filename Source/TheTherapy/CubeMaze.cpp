// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "CubeMaze.h"
#include <Components/BoxComponent.h>
#include <QofL/abbr.h>

// Sets default values
ACubeMaze::ACubeMaze() : root(CreateDefaultSubobject<USceneComponent>("root"))
{
  SetRootComponent(root);
  exitColliders[0] = CreateDefaultSubobject<UBoxComponent>("exit0");
  exitColliders[1] = CreateDefaultSubobject<UBoxComponent>("exit1");
  exitColliders[2] = CreateDefaultSubobject<UBoxComponent>("exit2");
  exitColliders[3] = CreateDefaultSubobject<UBoxComponent>("exit3");
  exitColliders[4] = CreateDefaultSubobject<UBoxComponent>("exit4");
  exitColliders[5] = CreateDefaultSubobject<UBoxComponent>("exit5");
  for (auto &e : exitColliders)
  {
    e->SetMobility(EComponentMobility::Movable);
    e->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
    e->InitBoxExtent(vec(120. / 2., 120. / 2., 120. / 2.));
  }

  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACubeMaze::BeginPlay()
{
  Super::BeginPlay();
  initialTransform = GetActorTransform();
}

// Called every frame
void ACubeMaze::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  auto time = GetWorld()->GetTimeSeconds();

  //  const auto move1 = FTransform{vec(120.0 * 12, 0., 0.)};
  //  const auto move2 = FTransform{vec(-120.0 * 12, 0., 0.)};
  //  const auto r = FTransform{rot(-50 * time, 0., 0.)};
  //
  //  auto m = FTransform{};
  //  FTransform::Multiply(&m, &move1, &initialTransform);
  //  auto n = FTransform{};
  //  FTransform::Multiply(&n, &r, &m);
  //  FTransform::Multiply(&m, &move2, &n);
  //  SetActorTransform(m);
}

auto ACubeMaze::OnConstruction(const FTransform &Transform) -> void
{
  Super::OnConstruction(Transform);
  setLoc(exitColliders[0],
         vec(120. * sz + 120. * 3. / 2., //
             120. / 2.,                  //
             120. / 2.));
  setLoc(exitColliders[1],
         vec(2 * 120. + 120. / 2., //
             120. / 2,             //
             120. * sz + 2 * 120. + 120. * 3. / 2.));
  setLoc(exitColliders[2],
         vec(120. * sz + 120. * 3. / 2. + 120. * 2., //
             120. * sz + 120. * 3. / 2.,             //
             120. * sz + 120. * 3. / 2. + 120. * 2.));
  setLoc(exitColliders[3],
         vec(120. * sz + 120. * 3. / 2. + 120. * 2., //
             120. * sz + 120. * 3. / 2. + 120 * 2.,  //
             120. + 120. * 3. / 2.));
  setLoc(exitColliders[4],
         vec(120. / 2.,                              //
             120. * sz + 120. * 3. / 2. + 120. * 2., //
             120. * sz + 120. * 3. / 2.));
  setLoc(exitColliders[5],
         vec(120. / 2., //
             120. / 2., //
             120. + 120. * 3. / 2.));
}
