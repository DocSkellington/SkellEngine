cmake_minimum_required(VERSION 3.12)

project(SkellEngine
    VERSION 0.1.0
    DESCRIPTION "2D homemade game engine using JSON and Lua to ease game development"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

enable_testing()

set(LIBRARY_NAME "SkellEngine")

add_subdirectory(engine)

option(SKELLENGINE_BUILD_TESTS "Whether to build the tests" OFF)
if(SKELLENGINE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

option(SKELLENGINE_BUILD_DOCUMENTATION "Whether to build the documentation (requires Doxygen)" OFF)
if (SKELLENGINE_BUILD_DOCUMENTATION)
    add_subdirectory(documentation)
endif()

option(SKELLENGINE_BUILD_EXAMPLES "Wether to build the examples" OFF)
if (SKELLENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()