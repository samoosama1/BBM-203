cmake_minimum_required(VERSION 3.26)
project(Exercises)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercises
        Counter.cpp
        Counter.h
        main.cpp)
