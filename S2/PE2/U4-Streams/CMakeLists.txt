cmake_minimum_required(VERSION 3.21)
project(U4-Streams)

set(CXX_STANDARD 20)

include_directories(.)

add_executable(${PROJECT_NAME}
        main.cpp Sale.h Sale.cpp)