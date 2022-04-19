cmake_minimum_required(VERSION 3.21)
project(U5-Overloading)

include_directories(.)

add_executable(U5-Overloading
        main.cpp
        vec2.cpp vec2.h ../../ALD/U4-5-BigO/Complexity.cpp ../../ALD/U4-5-BigO/Complexity.h)