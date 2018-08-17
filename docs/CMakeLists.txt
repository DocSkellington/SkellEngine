# Doxygen-lua
engine_set_option(ENGINE_LUADOC "" FILEPATH "The path to the ldoc.lua file. If the variable is not set, the Lua documentation is not generated.")

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # input and ouput files
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message("Doxygen must be installed to generate the documentation")
endif()

if (ENGINE_LUADOC)
    set(LDOC_WORKING_DIR ${CMAKE_SOURCE_DIR}/docs/lua)
    set(LDOC_IN ${LDOC_WORKING_DIR}/config.ld.in)
    set(LDOC_OUT ${LDOC_WORKING_DIR}/config.ld)
    configure_file(${LDOC_IN} ${LDOC_OUT} @ONLY)

    add_custom_target(doc_lua ALL
        COMMAND ${ENGINE_LUADOC} ${LDOC_WORKING_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating Lua API documentation with ldoc"
        VERBATIM
    )

    if (DOXYGEN_FOUND)
        add_dependencies(doc_doxygen doc_lua)
    endif()
else()
    message(STATUS "ENGINE_LUADOC not set; Lua API documentation will not be generated")
endif()