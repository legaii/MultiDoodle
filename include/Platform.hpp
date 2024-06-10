#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Rectangle.hpp"

namespace sj {
class Platform : public Rectangle {
  public:
    bool is_checkpoint;
    Platform(double x, double y, double width, double height, bool is_checkpoint);
};
}  // namespace sj

#endif
