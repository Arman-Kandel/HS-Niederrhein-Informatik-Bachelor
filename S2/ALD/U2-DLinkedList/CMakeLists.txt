cmake_minimum_required(VERSION 3.21)
project(U2-DLinkedList)

include_directories(.)

add_executable(U2-DLinkedList main.cpp DLinkedList.cpp DLinkedList.h DLinkedValue.cpp DLinkedValue.h)
