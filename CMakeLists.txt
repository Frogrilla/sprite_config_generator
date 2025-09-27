cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(sprite_config_generator)
add_executable(sprite_config_generator src/main.cpp)

target_include_directories(sprite_config_generator PRIVATE lib/extern)

find_library(libimago NAMES libimago REQUIRED)

target_link_libraries(sprite_config_generator ${libimago})
