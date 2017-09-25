# Import JSON for Modern C++ (https://github.com/nlohmann/json)
include(ExternalProject)

# Version and where to put the library
set(JSON_TAG v2.1.1)
set(JSON_HPP "${CMAKE_BINARY_DIR}/include/json.hpp")

file(DOWNLOAD https://github.com/nlohmann/json/releases/download/${JSON_TAG}/json.hpp ${JSON_HPP})

include_directories(${CMAKE_BINARY_DIR}/include)