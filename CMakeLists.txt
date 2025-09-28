cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(sprite_config_generator VERSION 1.0.1)
add_executable(sprite_config_generator src/main.cpp)
target_include_directories(sprite_config_generator PRIVATE include)

