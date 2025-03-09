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
    src/professor.cc
    src/time.cc
    src/bit_dp_solver.cc
    research_project/io.cc
    research_project/solver.cc
    concept_presentation/parameter.cc
    concept_presentation/slot.cc
    concept_presentation/io.cc
    concept_presentation/solver.cc
    bachelor_presentation/io.cc
    bachelor_presentation/solver.cc
    master_presentation/io.cc
    master_presentation/solver.cc
)

add_executable(solver ${SOURCES})