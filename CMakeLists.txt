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
include_directories("${PROJECT_BINARY_DIR}")

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

# Thor
find_package(Thor REQUIRED)
if(THOR_FOUND)
  include_directories(${THOR_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} thor)
endif()

#Lua
find_package(Lua 5.3 REQUIRED)
if (Lua_FOUND)
    include_directories(${LUA_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} lua5.3)
endif()

# Sol2
include(cmake_modules/Sol2.cmake)
add_dependencies(${EXECUTABLE_NAME} sol2)

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY media DESTINATION ${CMAKE_SOURCE_DIR}/bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${Engine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Engine_VERSION_MINOR}")
include(CPack)

# docs
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
  option(BUILD_DOC "Build documentation" ON)

  # Doxygen
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # input and ouput files
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    message(STATUS ${DOXYGEN_IN})

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
    )
  else()
    message("Doxygen must be installed to generate the documentation")
  endif()
endif()