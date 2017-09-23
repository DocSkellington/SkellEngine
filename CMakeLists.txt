cmake_minimum_required(VERSION 3.7)
project(Engine)
set (Engine_VERSION 0.0.1)

# C++17
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED on)

# Source files
set(SOURCE
    engine/src/main.cpp
)

# Includes
include_directories(engine/include)

add_executable(Engine ${SOURCE})