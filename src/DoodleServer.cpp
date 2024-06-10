#define CROW_MAIN
#include "crow.h"

#include "Server.hpp"


int main() {
  Server server;

  crow::SimpleApp app;

  CROW_ROUTE(app, "/add_room")([&]() -> std::string { return server.add_room(); });

  CROW_ROUTE(app, "/add_player/<string>")
  ([&](std::string room_id) -> std::string { return server.add_player(room_id); });

  CROW_ROUTE(app, "/start_room/<string>")
  ([&](std::string room_id) -> std::string {
    server.start_room(room_id);
    return "";
  });

  CROW_ROUTE(app, "/update_player/<string>/<string>/<string>")
  ([&](std::string room_id, std::string player_id,
       std::string player_state) -> std::string {
    auto load_res = sj::Load<sj::PlayerState>::function(player_state);
    if (!load_res.has_value()) {
      return "";
    }
    std::vector<std::pair<int, int>> players =
        server.update_player(room_id, player_id, load_res.value().result);
    std::string ans;
    for (const std::pair<int, int>& player : players) {
      ans += sj::Dump<int>::function(player.first);
      ans += sj::Dump<int>::function(player.second);
    }
    return ans;
  });

  app.port(18018).run();

  return 0;
}
