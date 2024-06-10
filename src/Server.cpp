#include "Server.hpp"


std::string Server::add_room() {
  std::string room_id = sj::random_string(16);
  rooms[room_id] = Room();
  return room_id;
}


std::string Server::add_player(std::string room_id) {
  std::map<std::string, Room>::iterator room = rooms.find(room_id);
  if (room == rooms.end() || room->second.started) {
    return "";
  }
  std::string player_id = sj::random_string(16);
  room->second.players[player_id] = sj::Player();
  return player_id;
}


void Server::start_room(std::string room_id) {
  std::map<std::string, Room>::iterator room = rooms.find(room_id);
  if (room == rooms.end() || room->second.started) {
    return;
  }
  room->second.started = true;
  for (std::pair<const std::string, sj::Player>& player : room->second.players) {
    player.second.start();
  }
}


std::vector<std::pair<int, int>> Server::update_player(std::string room_id,
                                                       std::string player_id,
                                                       sj::PlayerState player_state) {
  std::map<std::string, Room>::iterator room = rooms.find(room_id);
  if (room == rooms.end()) {
    return {};
  }
  std::map<std::string, sj::Player>::iterator me = room->second.players.find(player_id);
  if (me == room->second.players.end()) {
    return {};
  }
  if (room->second.started) {
    me->second.move(player_state);
  }
  std::vector<std::pair<int, int>> ans;
  ans.emplace_back((int)me->second.x, (int)me->second.y);
  for (const std::pair<const std::string, sj::Player>& player : room->second.players) {
    if (player.first != player_id) {
      ans.emplace_back((int)player.second.x, (int)player.second.y);
    }
  }
  return ans;
}
