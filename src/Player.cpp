#include "Player.hpp"

sj::Player::Player() : Rectangle(0.0, 0.0, player_width, player_height) {
}

void sj::Player::start() {
  t = std::chrono::steady_clock::now();
}

void sj::Player::move(PlayerState state) {
  for (std::chrono::time_point<std::chrono::steady_clock> now =
           std::chrono::steady_clock::now();
       t < now; t += dt_duration) {
    x += vx * dt;
    y += vy * dt + ay * dt * dt * 0.5;
    switch (state) {
      case PlayerState::Stay: {
        vx = 0.0;
      } break;
      case PlayerState::Left: {
        vx = -vx0;
      } break;
      case PlayerState::Right: {
        vx = vx0;
      } break;
    }
    offset = std::max(offset, y - window_height * 0.5);
    if (y < offset) {
      Platform checkpoint_platform = get_platforms(checkpoint.first)[checkpoint.second];
      x = checkpoint_platform.x;
      y = checkpoint_platform.y + checkpoint_platform.height;
      vy = 0.0;
      offset = checkpoint_platform.y;
    }
    bool collision = false;
    int min_level = (int)(y / level_height);
    int max_level = (int)((y + height) / level_height);
    for (int level = min_level; level <= max_level; ++level) {
      if (level < 0) {
        continue;
      }
      std::vector<Platform> platforms = get_platforms(level);
      for (int i = 0; i < (int)platforms.size(); ++i) {
        const Platform& platform = platforms[i];
        if (intersect(platform)) {
          if (platform.is_checkpoint) {
            checkpoint = {level, i};
          }
          collision = true;
          break;
        }
      }
      if (collision) {
        break;
      }
    }
    if (collision) {
      vy = vy0;
    } else {
      vy += ay * dt;
    }
  }
}
