cmake_minimum_required(VERSION 3.16)
project(right)

set(CMAKE_CXX_STANDARD 17)

add_executable(right main.cpp
        DFServer.cpp
        DFServer.h)
