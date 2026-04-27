cmake_minimum_required(VERSION 3.10)

project(ResearchProject)

# C++20を使用するように設定
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

include_directories(include)

set(SOURCES
    src/main.cc
    src/io_util.cc
    src/student.cc
    src/professor.cc
    src/time.cc
    src/bit_dp_solver.cc
    src/events/research_project/io.cc
    src/events/research_project/solver.cc
    src/events/concept_presentation/parameter.cc
    src/events/concept_presentation/slot.cc
    src/events/concept_presentation/io.cc
    src/events/concept_presentation/solver.cc
    src/events/bachelor_presentation/io.cc
    src/events/bachelor_presentation/solver.cc
    src/events/master_presentation/io.cc
    src/events/master_presentation/solver.cc
    src/events/intermediate_examination/io.cc
    src/events/intermediate_examination/solver.cc
)

add_executable(solver ${SOURCES})
