cmake_minimum_required(VERSION 3.17)
project(multi_doodle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DOODLE_FLAGS -Wall -Wextra -Wpedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND DOODLE_FLAGS
    -fsanitize=address,bounds,leak,undefined -fno-sanitize-recover -fstack-protector)
endif()

foreach(FLAG IN LISTS DOODLE_FLAGS)
  message(VERBOSE "Adding ${FLAG}")
  add_compile_options(${FLAG})
  add_link_options(${FLAG})
endforeach()

add_library(doodle_lib
  src/Common.cpp
  src/Parser.cpp
  src/Platform.cpp
  src/Player.cpp
  src/Rectangle.cpp
  src/Server.cpp
)

target_include_directories(doodle_lib PUBLIC include PUBLIC third_party)

add_executable(doodle src/Doodle.cpp)
add_executable(server src/DoodleServer.cpp)

find_package(Crow)
target_link_libraries(doodle doodle_lib raylib)
target_link_libraries(server doodle_lib Crow::Crow)
