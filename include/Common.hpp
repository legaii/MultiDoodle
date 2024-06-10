#ifndef COMMON_HPP
#define COMMON_HPP

#include "Parser.hpp"
#include "Platform.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace sj {

const double window_width = 400.0;
const double window_height = 700.0;

const double level_height = 100.0;

const double player_width = 20.0;
const double player_height = 20.0;

const double platform_width = 50.0;
const double platform_height = 10.0;

const std::chrono::milliseconds dt_duration(10);
const double dt =
    std::chrono::duration_cast<std::chrono::duration<double>>(dt_duration).count();

const double vx0 = 3.0e2;
const double vy0 = 7.0e2;

std::string random_string(int n);

std::vector<Platform> get_platforms(int level);

enum class PlayerState { Stay, Left, Right };

template <>
struct Load<PlayerState> {
    typedef std::optional<LoadResult<PlayerState>> ReturnT;
    static ReturnT function(const std::string& s);
};

template <>
struct Dump<PlayerState> {
    typedef std::string ReturnT;
    static ReturnT function(const PlayerState& t);
};

}  // namespace sj

#endif
