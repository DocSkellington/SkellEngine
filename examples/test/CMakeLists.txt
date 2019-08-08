configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)

set(SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/main.cpp

    TestSystem.cpp
)

add_executable(testEngine ${SOURCES})

target_link_libraries(testEngine PRIVATE ${LIBRARY_NAME})