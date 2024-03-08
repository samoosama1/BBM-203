cmake_minimum_required(VERSION 3.26)
project(AS0)

set(CMAKE_CXX_STANDARD 11)

add_executable(AS0 main.cpp
        util_functions.cpp
        util_functions.h
        curve_operations.cpp)
