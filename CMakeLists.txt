cmake_minimum_required(VERSION 3.10)

project(ResearchProject)

# C++20を使用するように設定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

set(SOURCES
    main.cc
    src/io_util.cc
    src/student.cc
)

add_executable(solver ${SOURCES})