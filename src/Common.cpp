#include "Common.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <random>

std::string sj::random_string(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<char> distrib('a', 'z');
  std::string ans;
  for (int i = 0; i < n; ++i) {
    ans.push_back(distrib(gen));
  }
  return ans;
}

std::vector<sj::Platform> sj::get_platforms(int level) {
  assert(level >= 0);
  if (level == 0) {
    return {Platform(0.0, 0.0, window_width, platform_height, true)};
  }
  std::mt19937 gen(level);
  std::vector<Platform> platforms;
  int n = std::uniform_int_distribution<int>(1, 2)(gen);
  for (int i = 0; i < n; ++i) {
    Platform new_platform(0.0, 0.0, platform_width, platform_height, false);
    for (bool collision = true; collision;) {
      new_platform.x = std::uniform_real_distribution<double>(
          0.0, window_width - new_platform.width)(gen);
      new_platform.y = std::uniform_real_distribution<double>(
          level_height * (double)level, level_height * (double)(level + 1))(gen);
      collision = false;
      for (const Platform& platform : platforms) {
        if (new_platform.intersect(platform)) {
          collision = true;
          break;
        }
      }
    }
    platforms.push_back(new_platform);
  }
  if (level % 100 == 0) {
    platforms.front().is_checkpoint = true;
  }
  return platforms;
}

sj::Load<sj::PlayerState>::ReturnT sj::Load<sj::PlayerState>::function(
    const std::string& s) {
  return FMap<Load<int>::ReturnT, Id<int, PlayerState>>::function(Load<int>::function(s));
}

sj::Dump<sj::PlayerState>::ReturnT sj::Dump<sj::PlayerState>::function(
    const PlayerState& t) {
  return Dump<int>::function(Id<PlayerState, int>::function(t));
}
