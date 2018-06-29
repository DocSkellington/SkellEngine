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

# Includes
include_directories(include)
include_directories("${PROJECT_BINARY_DIR}")

# ImGUI-SFML
set(IMGUI_ROOT ${CMAKE_SOURCE_DIR}/engine/dependencies/imgui)
set(IMGUI_SFML_ROOT ${CMAKE_SOURCE_DIR}/engine/dependencies/imgui-sfml)
add_subdirectory(dependencies/imgui-sfml)
include_directories(${IMGUI_ROOT} ${IMGUI_SFML_ROOT}) # Theoretically, IMGUI_SFML_INCLUDE_DIRS is supposed to work...
file(READ ${IMGUI_SFML_ROOT}/imconfig-SFML.h CONTENTS)
file(WRITE ${IMGUI_ROOT}/imconfig.h "${CONTENTS}")

file(GLOB
    IMGUI_SOURCES
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_SFML_ROOT}/imgui-SFML.cpp
)

# Executable
add_executable(${EXECUTABLE_NAME} ${IMGUI_SOURCES} ${SOURCES})

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