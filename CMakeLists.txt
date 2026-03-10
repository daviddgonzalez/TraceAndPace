cmake_minimum_required(VERSION 3.20)
project(TraceAndPace)

set(CMAKE_CXX_STANDARD 14)

include_directories(./backend)
include_directories(./frontend/cbuild)

set(SOURCE_FILES
        "./backend/BaseTree.hpp"
        "./backend/main.cpp"
)

add_executable(TraceAndPace ${SOURCE_FILES})