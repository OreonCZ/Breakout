cmake_minimum_required(VERSION 3.12)
project(breakoutC C)

add_executable(breakout src/main.c src/player.c src/text.c src/scene.c src/button.c src/blocks.c src/ball.c src/leaderboard.c)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(breakout SDL2 SDL2_ttf SDL2_mixer)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

file(COPY ${CMAKE_SOURCE_DIR}/sound DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/blocks.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})