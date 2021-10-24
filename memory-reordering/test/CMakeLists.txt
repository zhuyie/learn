cmake_minimum_required(VERSION 3.10)

project(reordering)

set(CMAKE_CXX_STANDARD 11)

add_executable(store_load_reordering 
    barrier.h
    store_load_reordering.cpp)

add_executable(peterson 
    barrier.h
    peterson.cpp)
