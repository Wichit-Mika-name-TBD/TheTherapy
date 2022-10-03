// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "HudUi.h"
#include <Components/Image.h>
#include <Components/ProgressBar.h>
#include <QofL/check_ret.h>
#include <QofL/get_prop.h>

auto UHudUi::setup() -> void
{
  AddToViewport();
}

auto UHudUi::setHearts(int val) -> void
{
  auto heart1 = getProp<UImage>(this, TEXT("Heart1"));
  CHECK_RET(heart1);
  auto heart2 = getProp<UImage>(this, TEXT("Heart2"));
  CHECK_RET(heart2);
  auto heart3 = getProp<UImage>(this, TEXT("Heart3"));
  CHECK_RET(heart3);
  auto heart4 = getProp<UImage>(this, TEXT("Heart4"));
  CHECK_RET(heart4);
  auto heart5 = getProp<UImage>(this, TEXT("Heart5"));
  CHECK_RET(heart5);
  auto heart6 = getProp<UImage>(this, TEXT("Heart6"));
  CHECK_RET(heart6);
  heart1->SetVisibility(val >= 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  heart2->SetVisibility(val >= 2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  heart3->SetVisibility(val >= 3 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  heart4->SetVisibility(val >= 4 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  heart5->SetVisibility(val >= 5 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  heart6->SetVisibility(val >= 6 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

auto UHudUi::setTime(float val) -> void
{
  auto timer = getProp<UProgressBar>(this, TEXT("Timer"));
  CHECK_RET(timer);
  timer->SetPercent(val / 10.f);
}
