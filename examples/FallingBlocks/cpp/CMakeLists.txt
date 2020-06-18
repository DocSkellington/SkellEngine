configure_file(main.cpp.in main.cpp)

set(SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
    GameState.cpp
    Grid.hpp
    Grid.cpp
)

add_executable(FallingBlocksCpp ${SOURCES})

target_link_libraries(FallingBlocksCpp SkellEngine)