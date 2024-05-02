cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)
project(heap VERSION 1.0 LANGUAGES CXX)

add_executable(heap src/heap.h src/tester.h src/main.cpp)