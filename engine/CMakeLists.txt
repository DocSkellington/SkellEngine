# C++17
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED on)

add_definitions(-Wall -Wextra)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# Source files
file(GLOB_RECURSE
    SOURCES
    src/*.cpp
    src/*.c
)

# Executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Includes
target_include_directories(${EXECUTABLE_NAME} PUBLIC include)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

# OpenGL
target_link_libraries(${EXECUTABLE_NAME} GL)

# Detect and add SFML
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-graphics sfml-window sfml-audio)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Thor
find_package(Thor REQUIRED)
if(THOR_FOUND)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${THOR_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} thor)
endif()

# TGUI
find_package(TGUI REQUIRED)
if (TGUI_FOUND)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${TGUI_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} tgui)
endif()

#Lua
find_package(Lua 5.3 REQUIRED)
if (Lua_FOUND)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${LUA_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} lua5.3)
endif()

# Sol2
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/CMakeLists.txt)
    # We force two options
    set(SINGLE ON CACHE BOOL "" FORCE)
    set(BUILD_LUA OFF CACHE BOOL "" FORCE)
    # We disable install for this subdirectory
    add_subdirectory(dependencies/sol2)
    
    add_dependencies(${EXECUTABLE_NAME} sol2_single)

    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/dependencies/sol2/single)
else()
    message(FATAL_ERROR "An external dependency (Sol2) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# JSON for modern C++
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/CMakeLists.txt)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/dependencies/json/single_include/nlohmann)
else()
    message(FATAL_ERROR "An external dependency (JSON) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# CPack packaging
# include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
# set(CPACK_PACKAGE_VERSION_MAJOR "${Engine_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${Engine_VERSION_MINOR}")
# include(CPack)