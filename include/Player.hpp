#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Common.hpp"
#include "Rectangle.hpp"

#include <chrono>

namespace sj {
class Player : public Rectangle {
  public:
    Player();
    void start();
    void move(PlayerState state);

  private:
    std::chrono::time_point<std::chrono::steady_clock> t;
    double vx = 0.0;
    double vy = 0.0;
    double ay = -1.0e3;
    double offset = 0.0;
    std::pair<int, int> checkpoint{0, 0};
};
}  // namespace sj

#endif
