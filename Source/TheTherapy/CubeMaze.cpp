// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "CubeMaze.h"
#include "Heart.h"
#include "HudUi.h"
#include "Maze.h"
#include "MobSpawner.h"
#include "PrjHud.h"
#include "TheTherapyCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/InstancedStaticMeshComponent.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <QofL/abbr.h>
#include <QofL/check_ret.h>
#include <QofL/log.h>
#include <QofL/obj_finder.h>
#include <Sound/SoundCue.h>

// Sets default values
ACubeMaze::ACubeMaze()
  : root(CreateDefaultSubobject<USceneComponent>("root")),
    mazeRegenSnd(OBJ_FINDER(SoundCue, "SFX", "SND_MazeRegen_Cue")),
    cubeRotationSnd(OBJ_FINDER(SoundCue, "SFX", "SND_CubeRotation_Cue")),
    winSnd(OBJ_FINDER(SoundCue, "SFX", "SND_Positive_Cue"))
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
    heart->OnDestroyed.AddDynamic(this, &ACubeMaze::onHeartDestroyed);
    hearts[i] = heart;
  }
  {
    FActorSpawnParameters param;
    param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    int i = 0;
    for (auto v : {
           vec(120. * (sz - 1) + 120. / 2., -120. * 2 + 120. / 2., 0.),
           vec(120. * 0. + 120. / 2., -120. * 2 + 120. / 2., 0.),
           vec(120. * (sz - 1 + 2) + 120. / 2., 120. / 2., 0.),
           vec(120. * 0. + 120. / 2., 120. * (sz - 1 + 2) + 120. / 2., 0.),
           vec(120. * -2. + 120. / 2., 120. * (sz - 1) + 120. / 2., 0.),
           vec(120. * (sz - 1) + 120. / 2., -120. * 2 + 120. / 2., 0.),
         })
    {
      mobSpawners[i] =
        GetWorld()->SpawnActor<AMobSpawner>(AMobSpawner::StaticClass(), v, rot(0., 0., 0.), param);
      mobSpawners[i]->AttachToActor(sides[i], FAttachmentTransformRules::KeepRelativeTransform);
      ++i;
    }
  }
  updateSidesVisibility();
  updateSidesCollision();
  nextRegen = 10.f;
  for (auto side : sides)
    side->regenMaze();
  auto hud = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->GetHUD<APrjHud>();
  CHECK_RET(hud);
  auto hudUi = hud->hudUi;
  CHECK_RET(hudUi);
  hudUi->nextLevel = nextLevel;
}

// Called every frame
void ACubeMaze::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  auto time = GetWorld()->GetTimeSeconds();

  const auto rotTime = 3.;
  const auto K = 90. / rotTime;
  auto m = FTransform{};
  auto n = FTransform{};
  switch (state)
  {
  case -1:
    m = initialTransform;
    angle = 90.;
    break;
  case 0: {
    const auto move1 = FTransform{vec(0., 0., 0.)};
    const auto move2 = FTransform{vec(0., 0., 0.)};
    if (angle > 90.)
    {
      angle = 90.;
      updateSidesCollision();
    }
    else if (angle == 90.) {}
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
      if (angle > 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else if (angle == 90.) {}
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
      if (angle > 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else if (angle == 90.) {}
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
      if (angle > 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else if (angle == 90.) {}
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
      if (angle > 90.)
      {
        angle = 90.;
        updateSidesCollision();
      }
      else if (angle == 90.) {}
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

  {
    auto controller = GetWorld()->GetFirstPlayerController();
    CHECK_RET(controller);
    auto player = Cast<ATheTherapyCharacter>(controller->GetPawnOrSpectator());
    CHECK_RET(player);

    if (state + 1 >= 0 && state + 1 < 6)
    {
      if (auto heart = hearts[state + 1])
      {
        player->setDistanceToTheGoal(FVector::Distance(getLoc(player), getLoc(heart)));
      }
      else if (auto exit = exitColliders[state + 1])
      {
        player->setDistanceToTheGoal(FVector::Distance(getLoc(player), exit->GetComponentLocation()));
      }
    }
    else
    {
      player->setDistanceToTheGoal(100.f);
    }
  }
  auto hud = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->GetHUD<APrjHud>();
  CHECK_RET(hud);
  auto hudUi = hud->hudUi;
  CHECK_RET(hudUi);
  if (state + 1 >= 0 && state + 1 < 6)
    hudUi->setTime(getTime());
  else
    hudUi->setTime(0.f);

  if (GetWorld()->GetTimeSeconds() > nextRegen && angle == 90.f)
  {
    nextRegen += 10.f;
    if (state + 1 >= 0 && state + 1 < 6)
      sides[state + 1]->regenMaze();
    UGameplayStatics::PlaySound2D(GetWorld(), mazeRegenSnd);
  }
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
  if (state + 1 >= 0 && state + 1 < 6)
    mobSpawners[state + 1]->despawn();

  if (state == 4)
  {
    UGameplayStatics::PlaySound2D(GetWorld(), winSnd);
    auto controller = GetWorld()->GetFirstPlayerController();
    CHECK_RET(controller);
    auto player = Cast<ATheTherapyCharacter>(controller->GetPawnOrSpectator());
    CHECK_RET(player);
    player->won();
    return;
  }

  state = idx;
  angle = 0.;
  LOG("state:", state, "hearts:", character->getHeartsCount());
  updateSidesVisibility();
  UGameplayStatics::PlaySound2D(GetWorld(), cubeRotationSnd);
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
  if (state + 1 >= 0 && state + 1 < 6)
    mobSpawners[state + 1]->spawn();
  resetNextRegen();
}

auto ACubeMaze::onHeartDestroyed(AActor *heart) -> void
{
  LOG("on heart destroyed");
  auto it = std::find(std::begin(hearts), std::end(hearts), heart);
  CHECK_RET(it != std::end(hearts));
  *it = nullptr;
}

auto ACubeMaze::resetNextRegen() -> void
{
  nextRegen = GetWorld()->GetTimeSeconds() + 10.f;
}

auto ACubeMaze::getTime() const -> float
{
  return nextRegen - GetWorld()->GetTimeSeconds();
}
