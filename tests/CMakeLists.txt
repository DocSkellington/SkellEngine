set(TESTS_SOURCES
    main.cpp

    systems/systems_tests.cpp
)

set(TESTS_NAME Engine-tests)
add_executable(${TESTS_NAME} ${TESTS_SOURCES})

message(STATUS ${CMAKE_SOURCE_DIR})
target_include_directories(${TESTS_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/single_include)

target_link_libraries(${TESTS_NAME} ${LIBRARY_NAME})

install(TARGETS ${TESTS_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
    ARCHIVE DESTINATION .
)