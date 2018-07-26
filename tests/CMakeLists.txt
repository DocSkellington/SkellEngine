set(TESTS_SOURCES
    main.cpp

    entities/entity_test.cpp
    entities/entity_manager_test.cpp

    files/gameDescription_tests.cpp

    systems/systems_tests.cpp

    utilities/json_lua_tests.cpp
)

set(TESTS_NAME SkellTests)
add_executable(${TESTS_NAME} ${TESTS_SOURCES})

message(STATUS ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/CMakeLists.tx)
if (EXISTS ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/CMakeLists.txt)
    target_include_directories(${TESTS_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/single_include)
else()
    message(FATAL_ERROR "An external dependency (Catch2) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

target_link_libraries(${TESTS_NAME} ${LIBRARY_NAME})

set(TESTS_PATH ${CMAKE_INSTALL_PREFIX}/tests)
install(TARGETS ${TESTS_NAME} RUNTIME DESTINATION ${TESTS_PATH})
install(DIRECTORY media DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:thor> DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:tgui> DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:SkellEngine> DESTINATION ${TESTS_PATH})