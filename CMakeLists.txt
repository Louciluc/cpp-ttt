cmake_minimum_required(VERSION 3.20)
project(tic-tac-toe LANGUAGES CXX)

add_executable(tic-tac-toe src/main.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_options(tic-tac-toe PRIVATE -Wall -Wextra -Wpedantic)
