cmake_minimum_required(VERSION 3.16)

project(test)

set(CMAKE_CXX_STANDARD 11)

add_executable(
    test-branch-prediction 
    test-branch-prediction.cpp)
