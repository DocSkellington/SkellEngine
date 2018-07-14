add_executable(test main.cpp)

target_link_libraries(test ${LIBRARY_NAME})
target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)

install(TARGETS ${EXECUTABLE_NAME} 
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
    ARCHIVE DESTINATION .
)
install(TARGETS test RUNTIME DESTINATION ${EXAMPLES_PATH}/test)
install(DIRECTORY media DESTINATION ${EXAMPLES_PATH}/test)
install(FILES $<TARGET_FILE:thor> DESTINATION ${EXAMPLES_PATH}/test)
install(FILES $<TARGET_FILE:tgui> DESTINATION ${EXAMPLES_PATH}/test)
install(FILES $<TARGET_FILE:SkellEngine> DESTINATION ${EXAMPLES_PATH}/test)