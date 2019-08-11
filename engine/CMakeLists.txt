# SFML
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)

# Lua
add_subdirectory(dependencies/lua)

# Sol2
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/single/CMakeLists.txt)
    message(FATAL_ERROR "An external dependency (Sol2) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# Local dependencies
# Thor
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/CMakeLists.txt)
    add_subdirectory(dependencies/Thor)
else()
    message(FATAL_ERROR "An external dependency (Thor) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# TGUI
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TGUI/CMakeLists.txt)
    set(TGUI_PKGCONFIG_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig CACHE FILEPATH "" FORCE)
    add_subdirectory(dependencies/TGUI)
else()
    message(FATAL_ERROR "An external dependency (TGUI) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# JSON for modern C++
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/CMakeLists.txt)
    message(FATAL_ERROR "An external dependency (JSON) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SkellEngine/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/SkellEngine/config.h
)

# Source files
set(SOURCES
    src/Engine.cpp
    src/Context.cpp

    src/entities/Entity.cpp
    src/entities/EntityManager.cpp

    src/entities/components/Component.cpp
    src/entities/components/ExternComponent.cpp
    src/entities/components/GraphicalSpriteSheetComponent.cpp
    src/entities/components/PositionComponent.cpp

    src/events/EventHandler.cpp
    src/events/Event.cpp
    src/events/ExternEvent.cpp
    src/events/StoreEventConnections.cpp
    src/events/EventConnection.cpp

    src/input/InputHandler.cpp
    src/input/detail/to_from_sfml_events.cpp
    src/input/detail/parse_json.cpp
    src/input/detail/write_json.cpp
    src/input/detail/default.cpp

    src/files/FileManager.cpp
    src/files/GameDescription.cpp
    src/files/StateDescription.cpp

    src/levels/Level.cpp
    src/levels/LevelDescription.cpp

    src/map/Layers.cpp
    src/map/Map.cpp

    src/shapes/EllipseShape.cpp

    src/states/GameState.cpp
    src/states/MainMenuState.cpp
    src/states/PauseState.cpp
    src/states/State.cpp
    src/states/StateManager.cpp
    src/states/StateContext.cpp

    src/systems/GraphicalOrthogonalSystem.cpp
    src/systems/GraphicalSystem.cpp
    src/systems/System.cpp
    src/systems/SystemManager.cpp
    src/systems/ExternSystem.cpp

    src/utilities/json_lua.cpp
    src/utilities/json_fusion.cpp
    src/utilities/MemberStorage.cpp

    src/tmxlite/FreeFuncs.cpp
    src/tmxlite/ImageLayer.cpp
    src/tmxlite/Map.cpp
    src/tmxlite/miniz.c
    src/tmxlite/Object.cpp
    src/tmxlite/ObjectGroup.cpp
    src/tmxlite/Property.cpp
    src/tmxlite/TileLayer.cpp
    src/tmxlite/Tileset.cpp

    src/tmxlite/detail/pugixml.cpp
)

# Library
add_library(${LIBRARY_NAME} SHARED
    ${SOURCES}
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/single/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/single_include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/extlibs/aurora/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TGUI/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        lua
        thor
        tgui
        GL
        sfml-system
        sfml-graphics
        sfml-window
        sfml-audio
)

target_compile_features(${LIBRARY_NAME}
    PUBLIC
        cxx_std_17
)

target_compile_options(${LIBRARY_NAME}
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall
            -Wextra
            -pedantic
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /Wall
        >
)

# std::filesystem for GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(${LIBRARY_NAME}
        PRIVATE
            stdc++fs
    )
endif()

# Thor
install(TARGETS thor
    EXPORT thorConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT thorConfig
    FILE thorConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/thor
)

export(EXPORT thorConfig
    FILE thorConfig.cmake
)

# sol
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/single/include/sol
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# JSON for modern C++
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/single_include/nlohmann
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# SkellEngine
install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/SkellEngine
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${LIBRARY_NAME}-targets
    FILE ${LIBRARY_NAME}-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

# Config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/${LIBRARY_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/${LIBRARY_NAME}Config-version.cmake
    VERSION ${SkellEngine_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/${LIBRARY_NAME}Config.cmake
        ${CMAKE_BINARY_DIR}/cmake/${LIBRARY_NAME}Config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

export(EXPORT ${LIBRARY_NAME}-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/${LIBRARY_NAME}-targets.cmake
)