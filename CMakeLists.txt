
cmake_minimum_required(VERSION 3.12)
project(sprite_config_generator)
add_executable(sprite_config_generator src/main.cpp)

find_package(nlohmann_json 3.12.0 REQUIRED)
find_library(libpng NAMES libpng.a PATHS /usr/local/lib REQUIRED)
find_library(libimago NAMES libimago.a PATHS /usr/local/lib REQUIRED)

target_link_libraries(sprite_config_generator ${libpng} ${libimago} nlohmann_json::nlohmann_json)
