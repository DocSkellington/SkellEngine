configure_file(${CMAKE_CURRENT_SOURCE_DIR}/events/event_handler_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/events/event_handler_test.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input/input_handler_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/input/input_handler_test.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/entities/entity_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/entities/entity_test.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/entities/entity_manager_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/entities/entity_manager_test.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/entities/components/component_setter_getter_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/entities/components/component_setter_getter_test.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/files/gameDescription_tests.cpp ${CMAKE_CURRENT_BINARY_DIR}/files/gameDescription_tests.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/files/fileManager_tests.cpp ${CMAKE_CURRENT_BINARY_DIR}/files/fileManager_tests.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/systems/systems_tests.cpp ${CMAKE_CURRENT_BINARY_DIR}/systems/systems_tests.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utilities/json_lua_tests.cpp ${CMAKE_CURRENT_BINARY_DIR}/utilities/json_lua_tests.cpp)

set(TESTS_SOURCES
    main.cpp

    math/vector2_test.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/events/event_handler_test.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/input/input_handler_test.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/entities/entity_test.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/entities/entity_manager_test.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/entities/components/component_setter_getter_test.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/files/gameDescription_tests.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/files/fileManager_tests.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/systems/systems_tests.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/utilities/json_lua_tests.cpp
)

set(TESTS_NAME SkellTests)

add_executable(${TESTS_NAME} ${TESTS_SOURCES})

target_include_directories(${TESTS_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/single_include/catch2
)

target_link_libraries(${TESTS_NAME}
    PRIVATE
        ${LIBRARY_NAME}
)

add_test(${TESTS_NAME} ${TESTS_NAME})