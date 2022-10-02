// Copyright (c) 2022 Wichit & Mika (name TBD)

#include "maze_gen.h"
#include <array>
#include <iomanip>
#include <sstream>
#include <string>

auto genMaze(int sz, int rmFraction) -> std::vector<bool>
{
  srand(time(nullptr));
  auto weights = std::vector<int>{};
  for (auto i = 0; i < sz * (2 * sz - 1); ++i)
    weights.push_back(exp(rand() % 1000 / 50.));

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
        const auto dx = d.first == 1 ? 0 : -1;
        const auto dy = d.second;
        const auto w = weights[x + dx + 2 * y + dy] + state[p].second;
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
    std::ostringstream l;
    for (auto x = 0; x < sz; ++x)
      l << std::setw(2) << state[x + y * sz].first;
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
            const auto dx = d.first == -1 ? -1 : 0;
            const auto dy = d.second;
            if (!ret[xx + dx + (2 * yy + dy) * sz])
            {
              wholePath = true;
              break;
            }
            ret[xx + dx + (2 * yy + dy) * sz] = false;
            xx = xxx;
            yy = yyy;
            break;
          }
        }
        if (wholePath)
          break;
      }
    }

  for (auto i = 0; i < (sz * sz * sz) * rmFraction / 100; ++i)
    ret[rand() % sz + rand() % (sz * 2) * sz] = false;

  for (auto y = 0; y < 2 * sz - 1; ++y)
  {
    std::string l;
    if (y % 2 == 0)
      l += " ";
    for (auto x = 0; x < sz - (y + 1) % 2; ++x)
      l += std::to_string(ret[x + y * sz]) + " ";
  }
  return ret;
}
