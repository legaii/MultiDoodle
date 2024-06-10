#include "Common.hpp"

#include "HTTPRequest.hpp"
#include "raylib.h"

#include <iomanip>
#include <iostream>


std::string get(std::string url) {
  http::Request req("http://127.0.0.1:18018" + url);
  const auto res = req.send("GET");
  return std::string(res.body.begin(), res.body.end());
}


std::string add_room() {
  return get("/add_room");
}


std::string add_player(std::string room_id) {
  return get("/add_player/" + room_id);
}


void start_room(std::string room_id) {
  get("/start_room/" + room_id);
}


std::vector<std::pair<int, int>> update_player(std::string room_id,
                                               std::string player_id,
                                               sj::PlayerState player_state) {
  std::string res = get("/update_player/" + room_id + "/" + player_id + "/" +
                        sj::Dump<sj::PlayerState>::function(player_state));
  std::vector<std::pair<int, int>> ans;
  for (int i = 0; i < (int)res.size();) {
    auto load_res = sj::Load<std::pair<int, int>>::function(res.substr(i)).value();
    ans.push_back(load_res.result);
    i += load_res.offset;
  }
  return ans;
}


std::vector<std::pair<int, int>> update_player_io(std::string room_id,
                                                  std::string player_id) {
  if (IsKeyDown(KEY_LEFT)) {
    if (IsKeyDown(KEY_RIGHT)) {
      return update_player(room_id, player_id, sj::PlayerState::Stay);
    }
    return update_player(room_id, player_id, sj::PlayerState::Left);
  }
  if (IsKeyDown(KEY_RIGHT)) {
    return update_player(room_id, player_id, sj::PlayerState::Right);
  }
  return update_player(room_id, player_id, sj::PlayerState::Stay);
}


int main() {
  const double font_size = 30.0;
  std::string room_id = add_room();
  std::string player_id = add_player(room_id);
  start_room(room_id);
  double offset = 0.0;
  InitWindow(sj::window_width, sj::window_height, "MultiDoodle");
  SetTargetFPS(20);

  while (!WindowShouldClose()) {
    std::vector<std::pair<int, int>> players = update_player_io(room_id, player_id);
    offset = std::max(offset, (double)players.front().second - sj::window_height * 0.5);
    offset = std::min(offset, (double)players.front().second);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(std::to_string((int)offset).c_str(), 0.0, sj::window_height - font_size,
             font_size, RAYWHITE);

    for (const std::pair<int, int>& player : players) {
      DrawRectangle(
          (double)player.first,
          sj::window_height - ((double)player.second + sj::player_height - offset),
          sj::player_width, sj::player_height, RAYWHITE);
    }

    for (int level = (int)((offset - sj::window_height) / sj::level_height);
         level <= (int)((offset + sj::window_height) / sj::level_height); ++level) {
      if (level < 0) {
        continue;
      }
      std::vector<sj::Platform> platforms = sj::get_platforms(level);
      for (const sj::Platform& platform : platforms) {
        DrawRectangle(
            platform.x, sj::window_height - (platform.y + platform.height - offset),
            platform.width, platform.height, platform.is_checkpoint ? MAGENTA : RAYWHITE);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
