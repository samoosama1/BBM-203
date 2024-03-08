cmake_minimum_required(VERSION 3.26)
project(AS4)

set(CMAKE_CXX_STANDARD 17)

add_executable(AS4 main.cpp
        Sector.cpp
        KD_Tree.cpp
        kNN.cpp
        SpaceSectorBST.cpp
        SpaceSectorLLRBT.cpp
        Test.cpp)
