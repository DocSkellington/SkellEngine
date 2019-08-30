configure_file(main.cpp.in main.cpp)

set(SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
    IntroState.cpp
)

add_executable(PacManCpp ${SOURCES})

target_link_libraries(PacManCpp SkellEngine)