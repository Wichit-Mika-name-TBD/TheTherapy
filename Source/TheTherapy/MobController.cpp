// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "MobController.h"
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
  MoveToActor(player, 50., true, true, false, nullptr, true);
}
