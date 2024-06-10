#include "Rectangle.hpp"

#include <algorithm>

sj::Rectangle::Rectangle(double x, double y, double width, double height)
    : x(x), y(y), width(width), height(height) {
}

bool sj::Rectangle::intersect(const sj::Rectangle& rectangle) {
  return std::max(x, rectangle.x) < std::min(x + width, rectangle.x + rectangle.width) &&
         std::max(y, rectangle.y) < std::min(y + height, rectangle.y + rectangle.height);
}
