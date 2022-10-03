// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "MobController.h"
#include <QofL/abbr.h>
#include <QofL/check_ret.h>

AMobController::AMobController()
{
  LOG("Mob is constucted");
  PrimaryActorTick.bCanEverTick = true;
}

auto AMobController::Tick(float dt) -> void
{
  Super::Tick(dt);
  auto status = GetMoveStatus();
  if (status == EPathFollowingStatus::Moving)
    return;
  auto controller = GetWorld()->GetFirstPlayerController();
  CHECK_RET(controller);
  auto player = controller->GetPawnOrSpectator();
  CHECK_RET(player);
  auto mob = GetPawn();
  auto distance = FVector::Distance(getLoc(mob), getLoc(player));
  if (distance > 100.)
    MoveToActor(player, 25., true, true, false, nullptr, true);
  else
  {
    LOG("try to attack the player");
    MoveToLocation(getLoc(mob) + (getLoc(player) - getLoc(mob)).GetSafeNormal(1.) * (distance + 20.),
                   0.,
                   true,
                   true,
                   true,
                   false,
                   nullptr,
                   true);
  }
}
