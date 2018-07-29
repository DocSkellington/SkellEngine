# Doxygen-lua
engine_set_option(DOXYGEN_LUA "" FILEPATH "The path to the lua2dox executable. If the variable is not set, the Lua documentation is not generated.")

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  # input and ouput files
  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  
  add_custom_target(doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
  #add_dependencies(doc_doxygen ${EXECUTABLE_NAME})
else()
  message("Doxygen must be installed to generate the documentation")
endif()