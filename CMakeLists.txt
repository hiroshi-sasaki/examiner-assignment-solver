cmake_minimum_required(VERSION 3.10)

project(ResearchProject)

# C++20を使用するように設定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(solver main.cpp)
