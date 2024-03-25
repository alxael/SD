cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)
project(SD VERSION 1.0 LANGUAGES CXX)

add_executable(SD src/ds.h src/tester.h src/main.cpp)