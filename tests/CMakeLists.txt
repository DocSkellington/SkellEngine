set(TESTS_SOURCES
    main.cpp

    entities/entity_test.cpp
    entities/entity_manager_test.cpp
    entities/components/component_setter_getter_test.cpp

    files/gameDescription_tests.cpp

    systems/systems_tests.cpp

    utilities/json_lua_tests.cpp
)

set(TESTS_NAME SkellTests)

add_executable(${TESTS_NAME} ${TESTS_SOURCES})

target_include_directories(${TESTS_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/single_include
)

target_link_libraries(${TESTS_NAME}
    PRIVATE
        ${LIBRARY_NAME}
)

add_test(${TESTS_NAME} ${TESTS_NAME})