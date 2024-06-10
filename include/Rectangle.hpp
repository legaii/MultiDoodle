#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

namespace sj {
class Rectangle {
  public:
    double x;
    double y;
    double width;
    double height;
    Rectangle(double x, double y, double width, double height);
    bool intersect(const Rectangle& rectangle);
};
}  // namespace sj

#endif
