// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "PrjHud.h"
#include "HudUi.h"
#include <Blueprint/UserWidget.h>
#include <QofL/check_ret.h>
#include <QofL/class_finder.h>

APrjHud::APrjHud()
  : hudUi(CreateWidget<UHudUi>(GetWorld(), CLASS_FINDER(UUserWidget, "BP", "BP_HudUi")))
{
}

auto APrjHud::BeginPlay() -> void
{
  Super::BeginPlay();
  CHECK_RET(hudUi);
  hudUi->setup();
}
