cmake_minimum_required(VERSION 3.20)
project(TraceAndPace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug") # or "Release"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/frontend/src/wasm")


include_directories(./backend)
include_directories(./frontend/cbuild)
include_directories(./frontend/src/wasm)

set(SOURCE_FILES
        backend/BaseTree.hpp
        backend/main.cpp
        backend/AVLTree.hpp
        backend/BTree.hpp
)

add_executable(TraceAndPace ${SOURCE_FILES})

# Flags for when running emcc
set_target_properties(TraceAndPace PROPERTIES
        SUFFIX ".mjs"
        LINK_FLAGS "-g -s SINGLE_FILE=1 -s NO_EXIT_RUNTIME=1 -s STANDALONE_WASM=0 --embed-file ${CMAKE_CURRENT_SOURCE_DIR}/frontend/src/assets@/assets --bind --emit-tsd TraceAndPace.d.mts"
)