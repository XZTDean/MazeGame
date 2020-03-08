cmake_minimum_required(VERSION 3.15)
project(MazeGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(MazeGame main.cpp Map.cpp Map.h Coordinate.h UI.h)