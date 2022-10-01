// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "maze_gen.h"
#include <QofL/log.h>
#include <array>
#include <string>

auto genMaze(int sz) -> std::vector<bool>
{
  srand(time(nullptr));
  auto weights = std::vector<int>{};
  for (auto i = 0; i < sz * (2 * sz - 1); ++i)
    weights.push_back(rand() % 100);

  auto state = std::vector<std::pair<int, int>>{};
  for (auto i = 0; i < sz * sz; ++i)
    state.push_back(std::make_pair(-1, 0));

  auto wave = std::vector<int>{};
  wave.push_back(0);
  state[0] = std::make_pair(0, 0);
  auto newWave = std::vector<int>{};
  while (!wave.empty())
  {
    for (const auto p : wave)
    {
      const auto x = p % sz;
      const auto y = p / sz;
      for (auto d : std::array{std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1}})
      {
        const auto xx = x + d.first;
        const auto yy = y + d.second;
        if (xx < 0 || xx >= sz || yy < 0 || yy >= sz)
          continue;
        const auto pp = xx + yy * sz;
        const auto w = weights[x + d.first + 2 * y + d.second] + state[p].second;
        if (state[pp].first != -1 && state[pp].second <= w)
          continue;
        state[pp].second = w;
        state[pp].first = state[p].first + 1;
        newWave.push_back(pp);
      }
    }
    wave = newWave;
    newWave.clear();
  }

  for (auto y = 0; y < sz; ++y)
  {
    std::string l;
    for (auto x = 0; x < sz; ++x)
      l += std::to_string(state[x + y * sz].first);
    LOG(l);
  }

  auto ret = std::vector<bool>{};
  for (auto i = 0; i < sz * (2 * sz - 1); ++i)
    ret.push_back(true);

  for (auto y = 0; y < sz; ++y)
    for (auto x = 0; x < sz; ++x)
    {
      auto xx = x;
      auto yy = y;
      auto wholePath = false;
      while (xx != 0 || yy != 0)
      {
        const auto p = xx + yy * sz;
        const auto n = state[p].first;
        for (auto d : std::array{std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1}})
        {
          const auto xxx = xx + d.first;
          const auto yyy = yy + d.second;
          if (xxx < 0 || xxx >= sz || yyy < 0 || yyy >= sz)
            continue;
          const auto pp = xxx + yyy * sz;
          const auto nn = state[pp].first;
          if (nn == n - 1)
          {
            if (!ret[xx + d.first + (2 * yy + d.second) * sz])
            {
              wholePath = true;
              break;
            }
            ret[xx + d.first + (2 * yy + d.second) * sz] = false;
            xx = xxx;
            yy = yyy;
            break;
          }
        }
        if (wholePath)
          break;
      }
    }

  for (auto y = 0; y < 2 * sz - 1; ++y)
  {
    std::string l;
    if (y % 2 == 0)
      l += " ";
    for (auto x = 0; x < sz - (y + 1) % 2; ++x)
      l += std::to_string(ret[x + y * sz]) + " ";
    LOG(l);
  }
  return ret;
}
