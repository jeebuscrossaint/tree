cmake_minimum_required(VERSION 4.1)
project(tree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "g++")

# Static linking to avoid DLL issues
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)
include_directories(src)

add_executable(tree
        src/main.cpp
        src/flags.cpp
        src/flags.h
        src/files.cpp
        src/files.h
        src/env.cpp
        src/env.h)
