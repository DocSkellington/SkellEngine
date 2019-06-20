find_package(Doxygen
    OPTIONAL_COMPONENTS dot mscgen dia
)

if (DOXYGEN_FOUND)
    set(DOXYGEN_EXCLUDE_PATTERNS "*/tmxlite/*")
    set(DOXYGEN_SOURCE_BROWSER TRUE)

    doxygen_add_docs(documentation
        .
        ${CMAKE_SOURCE_DIR}/engine/include
        ${CMAKE_SOURCE_DIR}/engine/src
    )

    find_program(LUA_EXECUTABLE lua)

    set(LDOC_EXECUTABLE ${CMAKE_SOURCE_DIR}/engine/dependencies/LDoc/ldoc.lua)

    if (LDOC_EXECUTABLE AND LUA_EXECUTABLE)
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/lua/config.ld.in
            ${CMAKE_CURRENT_BINARY_DIR}/config.ld @ONLY
        )

        add_custom_target(lua_documentation
            COMMAND ${LUA_EXECUTABLE} ${LDOC_EXECUTABLE} .
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Lua API documentation with ldoc"
            VERBATIM
        )

        add_dependencies(documentation lua_documentation)
    else()
        message(STATUS "LDOC_EXECUTABLE or LUA_EXECUTABLE is not set. The Lua documentation will not be built.")
    endif()
endif()