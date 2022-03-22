cmake_minimum_required(VERSION 3.21)
project(HS_Niederrhein_Informatik_Bachelor C)

set(CMAKE_C_STANDARD 11)

include_directories(S2/PE2/U1-Modular-Programming)

add_executable(HS_Niederrhein_Informatik_Bachelor
        S2/PE2/U1-Modular-Programming/liste.c
        S2/PE2/U1-Modular-Programming/liste.h
        S2/PE2/U1-Modular-Programming/main.c
        S2/PE2/U1-Modular-Programming/maths.c
        S2/PE2/U1-Modular-Programming/maths.h S2/PE2/U1-Modular-Programming/stack.c S2/PE2/U1-Modular-Programming/stack.h S2/PE2/U1-Modular-Programming/sort.c S2/PE2/U1-Modular-Programming/sort.h)
