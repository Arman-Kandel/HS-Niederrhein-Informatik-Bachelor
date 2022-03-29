cmake_minimum_required(VERSION 3.21)
project(U1-Modular-Programming C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(U1-Modular-Programming
        liste.c
        liste.h
        main.c
        maths.c
        maths.h stack.c stack.h sort.c sort.h)
