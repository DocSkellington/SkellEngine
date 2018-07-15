set(TESTS_SOURCES
    main.cpp

    systems/systems_tests.cpp

    FileManager/gameDescription_tests.cpp

    utilities/json_lua_tests.cpp
)

set(TESTS_NAME SkellTests)
add_executable(${TESTS_NAME} ${TESTS_SOURCES})

message(STATUS ${CMAKE_SOURCE_DIR})
target_include_directories(${TESTS_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/dependencies/Catch2/single_include)

target_link_libraries(${TESTS_NAME} ${LIBRARY_NAME})

set(TESTS_PATH ${CMAKE_INSTALL_PREFIX}/tests)
install(TARGETS ${TESTS_NAME} RUNTIME DESTINATION ${TESTS_PATH})
install(DIRECTORY media DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:thor> DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:tgui> DESTINATION ${TESTS_PATH})
install(FILES $<TARGET_FILE:SkellEngine> DESTINATION ${TESTS_PATH})