# This file is partialy copied from https://github.com/SFML/SFML/wiki/Tutorial%3A-Build-your-SFML-project-with-CMake
cmake_minimum_required(VERSION 3.11)

# A macro to help defining an option (from https://github.com/SFML/SFML/blob/master/CMakeLists.txt)
macro(engine_set_option var default type docstring)
  if(NOT DEFINED ${var})
      set(${var} ${default})
  endif()
  set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# Debug or Release?
engine_set_option(CMAKE_BUILD_TYPE Debug STRING "Choose the type of the build (Debug or Release)")

# Project name
project(Engine)

# Version info
set(Engine_VERSION_MAJOR 0)
set(Engine_VERSION_MINOR 0)
set(Engine_VERSION_REVISION 1)
# Set version information in a config.h file
configure_file(
  "engine/include/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# Should the docs be built?
engine_set_option(ENGINE_BUILD_DOC FALSE BOOL "TRUE to generate documentation, FALSE to ignore it")

set(EXECUTABLE_NAME "Engine")
add_subdirectory(engine)
if (ENGINE_BUILD_DOC)
  add_subdirectory(docs)
endif()

# Install target
install(PROGRAMS ${CMAKE_BINARY_DIR}/engine/${EXECUTABLE_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY media DESTINATION ${CMAKE_SOURCE_DIR}/bin)