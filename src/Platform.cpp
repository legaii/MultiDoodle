#include "Platform.hpp"

sj::Platform::Platform(double x,
                       double y,
                       double width,
                       double height,
                       bool is_checkpoint)
    : Rectangle(x, y, width, height), is_checkpoint(is_checkpoint) {
}
