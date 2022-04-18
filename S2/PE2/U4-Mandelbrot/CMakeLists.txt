cmake_minimum_required(VERSION 3.21)
project(U4-Mandelbrot)

include_directories(.)

add_executable(U4-Mandelbrot
        main.cpp
        Complex.cpp Complex.h)