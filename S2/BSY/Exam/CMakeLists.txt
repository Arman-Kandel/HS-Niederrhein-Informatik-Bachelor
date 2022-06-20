cmake_minimum_required(VERSION 3.21)
project(Exam)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(${PROJECT_NAME}
        main.c
        wiringPi.h time-utils.h)

# Below is required for thread libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
