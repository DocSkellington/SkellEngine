# C++17
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED on)

add_definitions(-Wall)

# Source files
file(GLOB_RECURSE
    SOURCES
    src/*.cpp
)

# Includes
include_directories(include)
include_directories("${PROJECT_BINARY_DIR}")

# Executable
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

# tmx-loader
find_package(TMX REQUIRED)
if(TMX_FOUND)
    include_directories(${TMX_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} tmx-loader pugi)
endif()

#Lua
find_package(Lua 5.3 REQUIRED)
if (Lua_FOUND)
    include_directories(${LUA_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} lua5.3)
endif()

# Sol2
include(../cmake_modules/Sol2.cmake)
add_dependencies(${EXECUTABLE_NAME} sol2)

# JSON for modern C++
include(../cmake_modules/JSON.cmake)

# CPack packaging
# include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
# set(CPACK_PACKAGE_VERSION_MAJOR "${Engine_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${Engine_VERSION_MINOR}")
# include(CPack)