// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "CubeMaze.h"
#include "Heart.h"
#include "Maze.h"
#include "TheTherapyCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/InstancedStaticMeshComponent.h>
#include <QofL/abbr.h>
#include <QofL/log.h>

// Sets default values
ACubeMaze::ACubeMaze() : root(CreateDefaultSubobject<USceneComponent>("root"))
{
  SetRootComponent(root);
  exitCollider0 = CreateDefaultSubobject<UBoxComponent>("exit0");
  exitCollider1 = CreateDefaultSubobject<UBoxComponent>("exit1");
  exitCollider2 = CreateDefaultSubobject<UBoxComponent>("exit2");
  exitCollider3 = CreateDefaultSubobject<UBoxComponent>("exit3");
  exitCollider4 = CreateDefaultSubobject<UBoxComponent>("exit4");
  exitCollider5 = CreateDefaultSubobject<UBoxComponent>("exit5");
  exitColliders[0] = exitCollider0;
  exitColliders[1] = exitCollider1;
  exitColliders[2] = exitCollider2;
  exitColliders[3] = exitCollider3;
  exitColliders[4] = exitCollider4;
  exitColliders[5] = exitCollider5;
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
  for (auto &e : exitColliders)
  {
    e->OnComponentBeginOverlap.AddDynamic(this, &ACubeMaze::onOverlap);
  }
  state = -1;
  angle = 0.f;
  updateSidesVisibility();
  updateSidesCollision();
  for (auto i = 0; i < 6; ++i)
  {
    FActorSpawnParameters param;
    param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    auto heart = GetWorld()->SpawnActor<AHeart>(AHeart::StaticClass(),
                                                vec(120. * (rand() % sz) + 120. / 2., //
                                                    120. * (rand() % sz) + 120. / 2., //
                                                    15.),
                                                rot(0., 0., 90.),
                                                param);
    heart->AttachToActor(sides[i], FAttachmentTransformRules::KeepRelativeTransform);
  }
}

// Called every frame
void ACubeMaze::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  auto time = GetWorld()->GetTimeSeconds();

  const auto K = 20.;
  auto m = FTransform{};
  auto n = FTransform{};
  switch (state)
  {
  case -1: m = initialTransform; break;
  case 0: {
    const auto move1 = FTransform{vec(0., 0., 0.)};
    const auto move2 = FTransform{vec(0., 0., 0.)};
    if (angle >= 90.)
    {
      angle = 90.;
      updateSidesCollision();
    }
    else
      angle += K * DeltaTime;
    const auto r = FTransform{rot(0., 0., -angle)};
    FTransform::Multiply(&m, &move1, &initialTransform);
    FTransform::Multiply(&n, &r, &m);
    FTransform::Multiply(&m, &move2, &n);
    break;
  }
  case 1: {
    {
      const auto move1 = FTransform{vec(0., 0., 0.)};
      const auto move2 = FTransform{vec(0., 0., 0.)};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&m, &move1, &initialTransform);
      FTransform::Multiply(&n, &r, &m);
      FTransform::Multiply(&m, &move2, &n);
    }
    {
      const auto move3 = FTransform{vec(0., 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(0., 0., -120. * (4. + sz))};
      if (angle >= 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else
        angle += K * DeltaTime;
      const auto r = FTransform{rot(0., 0., -angle)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    break;
  }
  case 2: {
    {
      const auto move1 = FTransform{vec(0., 0., 0.)};
      const auto move2 = FTransform{vec(0., 0., 0.)};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&m, &move1, &initialTransform);
      FTransform::Multiply(&n, &r, &m);
      FTransform::Multiply(&m, &move2, &n);
    }
    {
      const auto move3 = FTransform{vec(0., 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(0., 0., -120. * (4. + sz))};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(120. * (4. + sz), 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(-120. * (4. + sz), 0., -120. * (4. + sz))};
      if (angle >= 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else
        angle += K * DeltaTime;
      const auto r = FTransform{rot(angle, 0., 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    break;
  }
  case 3: {
    {
      const auto move1 = FTransform{vec(0., 0., 0.)};
      const auto move2 = FTransform{vec(0., 0., 0.)};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&m, &move1, &initialTransform);
      FTransform::Multiply(&n, &r, &m);
      FTransform::Multiply(&m, &move2, &n);
    }
    {
      const auto move3 = FTransform{vec(0., 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(0., 0., -120. * (4. + sz))};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(120. * (4. + sz), 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(-120. * (4. + sz), 0., -120. * (4. + sz))};
      const auto r = FTransform{rot(90., 0., 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(120. * (4. + sz), 120. * (4. + sz), 0.)};
      const auto move4 = FTransform{vec(-120. * (4. + sz), -120. * (4. + sz), 0.)};
      if (angle >= 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else
        angle += K * DeltaTime;
      const auto r = FTransform{rot(0., -angle, 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    break;
  }
  case 4: {
    {
      const auto move1 = FTransform{vec(0., 0., 0.)};
      const auto move2 = FTransform{vec(0., 0., 0.)};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&m, &move1, &initialTransform);
      FTransform::Multiply(&n, &r, &m);
      FTransform::Multiply(&m, &move2, &n);
    }
    {
      const auto move3 = FTransform{vec(0., 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(0., 0., -120. * (4. + sz))};
      const auto r = FTransform{rot(0., 0., -90.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(120. * (4. + sz), 0., 120. * (4. + sz))};
      const auto move4 = FTransform{vec(-120. * (4. + sz), 0., -120. * (4. + sz))};
      const auto r = FTransform{rot(90., 0., 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(120. * (4. + sz), 120. * (4. + sz), 0.)};
      const auto move4 = FTransform{vec(-120. * (4. + sz), -120. * (4. + sz), 0.)};
      const auto r = FTransform{rot(0., -90., 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    {
      const auto move3 = FTransform{vec(0., 120. * (4. + sz), 0.)};
      const auto move4 = FTransform{vec(0., -120. * (4. + sz), 0.)};
      if (angle >= 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else
        angle += K * DeltaTime;
      const auto r = FTransform{rot(0., -angle, 0.)};
      FTransform::Multiply(&n, &move3, &m);
      FTransform::Multiply(&m, &r, &n);
      FTransform::Multiply(&n, &move4, &m);
      m = n;
    }
    break;
  }
  }

  SetActorTransform(m);
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

void ACubeMaze::onOverlap(UPrimitiveComponent *HitComponent,
                          AActor *OtherActor,
                          UPrimitiveComponent *OtherComp,
                          int n,
                          bool b,
                          const FHitResult &Hit)
{
  auto character = Cast<ATheTherapyCharacter>(OtherActor);
  if (!character)
    return;
  LOG("onOverlap");
  auto it = std::find(std::begin(exitColliders), std::end(exitColliders), HitComponent);
  if (it == std::end(exitColliders))
    return;
  auto idx = std::distance(std::begin(exitColliders), it);
  if (state == idx)
    return;

  if (character->getHeartsCount() < idx + 1)
  {
    LOG("We need to collect a heart first");
    return;
  }
  state = idx;
  angle = 0.;
  LOG("state:", state, "hearts:", character->getHeartsCount());
  updateSidesVisibility();
}

auto ACubeMaze::updateSidesVisibility() -> void
{
  for (auto i = 0; i < 6; ++i)
    sides[i]->SetActorHiddenInGame(false);
  switch (state)
  {
  case -1:
    sides[2]->SetActorHiddenInGame(true);
    sides[4]->SetActorHiddenInGame(true);
    break;
  case 0:
    sides[4]->SetActorHiddenInGame(true);
    sides[5]->SetActorHiddenInGame(true);
    break;
  case 1:
    sides[0]->SetActorHiddenInGame(true);
    sides[5]->SetActorHiddenInGame(true);
    break;
  case 2:
    sides[1]->SetActorHiddenInGame(true);
    sides[5]->SetActorHiddenInGame(true);
    break;
  case 3:
    sides[1]->SetActorHiddenInGame(true);
    sides[0]->SetActorHiddenInGame(true);
    break;
  case 4:
    sides[0]->SetActorHiddenInGame(true);
    sides[3]->SetActorHiddenInGame(true);
    break;
  case 5: break;
  }
}

auto ACubeMaze::updateSidesCollision() -> void
{
  for (auto i = 0; i < 6; ++i)
  {
    auto side = sides[i];
    auto isVisible = !side->IsHidden();
    TArray<UActorComponent *> comps;
    side->GetComponents(UInstancedStaticMeshComponent::StaticClass(), comps, true);
    for (auto comp : comps)
    {
      auto mesh = Cast<UInstancedStaticMeshComponent>(comp);
      if (!mesh)
        continue;
      if (isVisible)
        mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
      else
        mesh->SetCollisionProfileName(TEXT("NoCollision"));
    }
  }
}
