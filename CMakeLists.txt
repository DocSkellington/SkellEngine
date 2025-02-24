# This file is partialy copied from https://github.com/SFML/SFML/wiki/Tutorial%3A-Build-your-SFML-project-with-CMake
cmake_minimum_required(VERSION 3.7)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(Engine)

# Set version information in a config.h file
set(Engine_VERSION_MAJOR 0)
set(Engine_VERSION_MINOR 1)
configure_file(
  "engine/include/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# C++17
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED on)

# Source files
file(GLOB_RECURSE
    SOURCES
    engine/src/*.cpp
)

# Includes
include_directories(engine/include)

set(EXECUTABLE_NAME "Engine")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-graphics sfml-window sfml-audio ${SFML_DEPENDENCIES})
endif()

#Lua
find_package(Lua 5.3 REQUIRED)
if (Lua_FOUND)
    include_directories(${LUA_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} lua5.3)
endif()

# Sol2
include(cmake_modules/Sol2.cmake)

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${Engine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Engine_VERSION_MINOR}")
include(CPack)