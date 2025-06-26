cmake_minimum_required(VERSION 3.31)
project(assignment_4_plpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(assignment_4_plpp main.cpp
        src/line.cpp
        src/line.h
        func/functionality.cpp)
