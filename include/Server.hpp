#ifndef SERVER_HPP
#define SERVER_HPP

#include "Common.hpp"
#include "Player.hpp"

#include <map>

class Room {
  public:
    std::map<std::string, sj::Player> players;
    bool started = false;
};


class Server {
  public:
    std::string add_room();
    std::string add_player(std::string room_id);
    void start_room(std::string room_id);
    std::vector<std::pair<int, int>> update_player(std::string room_id,
                                                   std::string player_id,
                                                   sj::PlayerState player_state);

  private:
    std::map<std::string, Room> rooms;
};

#endif
