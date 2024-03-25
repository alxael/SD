cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)
project(sort VERSION 1.0 LANGUAGES CXX)

add_executable(sort src/ds.h src/tester.h src/main.cpp)