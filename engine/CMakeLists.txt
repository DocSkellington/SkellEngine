# SFML
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)

# Lua
add_subdirectory(dependencies/lua)

# Sol2
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/single/CMakeLists.txt)
    message(FATAL_ERROR "An internal dependency (Sol2) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# tmxlite
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/CMakeLists.txt)
    # Since the tmxlite's CMakeLists.txt is badly written, we need to manually compile the library
    add_library(tmxlite SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/detail/pugixml.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/FreeFuncs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/ImageLayer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/Map.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/miniz.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/Object.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/ObjectGroup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/Property.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/TileLayer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/LayerGroup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/src/Tileset.cpp
    )
    target_include_directories(tmxlite
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/include>
            $<INSTALL_INTERFACE:include>
    )
else()
    message(FATAL_ERROR "An internal dependency () could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# Thor
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/CMakeLists.txt)
    # Since the Thor's CMakeLists.txt is badly written, we need to manually compile the library
    add_library(thor SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Action.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ActionOperations.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Affectors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Arrow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/BigSprite.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/BigTexture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/CallbackTimer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ColorAnimation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ColorGradient.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ConcaveShape.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Distributions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Emitters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/FadeAnimation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/FrameAnimation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/InputNames.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Joystick.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Particle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ParticleSystem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Random.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Shapes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/StopWatch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Timer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/ToString.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Triangulation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/Trigonometry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/src/UniformAccess.cpp
    )
    target_include_directories(thor
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/extlibs/aurora/include>
    )
else()
    message(FATAL_ERROR "An internal dependency (Thor) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# TGUI
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TGUI/CMakeLists.txt)
    set(TGUI_PKGCONFIG_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig CACHE FILEPATH "" FORCE)
    add_subdirectory(dependencies/TGUI)
else()
    message(FATAL_ERROR "An internal dependency (TGUI) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# JSON for modern C++
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/CMakeLists.txt)
    message(FATAL_ERROR "An internal dependency (JSON) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SkellEngine/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/SkellEngine/config.h
)

# Source files
set(SOURCES
    src/Engine.cpp
    src/Context.cpp
    src/Logger.cpp

    src/entities/Entity.cpp
    src/entities/EntityManager.cpp

    src/entities/components/Component.cpp
    src/entities/components/ExternComponent.cpp
    src/entities/components/GraphicalSpriteSheetComponent.cpp
    src/entities/components/PositionComponent.cpp
    src/entities/components/AnimationComponent.cpp

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

    src/levels/Level.cpp
    src/levels/LevelDescription.cpp

    src/map/Layers.cpp
    src/map/Map.cpp

    src/shapes/EllipseShape.cpp

    src/states/State.cpp
    src/states/ExternState.cpp
    src/states/StateManager.cpp
    src/states/StateContext.cpp

    src/systems/GraphicalOrthogonalSystem.cpp
    src/systems/GraphicalSystem.cpp
    src/systems/System.cpp
    src/systems/SystemManager.cpp
    src/systems/ExternSystem.cpp
    src/systems/AnimationSystem.cpp

    src/animations/FrameAnimation.cpp
    src/animations/ColorAnimation.cpp
    src/animations/FadeAnimation.cpp
    src/animations/detail/to_from_sfml_color.cpp

    src/utilities/json_lua.cpp
    src/utilities/json_fusion.cpp
    src/utilities/lua_sfml.cpp
    src/utilities/lua_gui.cpp
    src/utilities/MemberStorage.cpp
    src/utilities/ExternMemberStorage.cpp
)

# Library
add_library(${LIBRARY_NAME} SHARED
    ${SOURCES}
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/include>
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
        tmxlite
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

# std::filesystem for GCC and Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(${LIBRARY_NAME}
        PRIVATE
            stdc++fs
    )
endif()

# Lua
install(TARGETS lua
    EXPORT luaTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT luaTargets
    FILE luaConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lua
)

# tmxlite
install(TARGETS tmxlite
    EXPORT tmxliteTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT tmxliteTargets
    FILE tmxliteConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tmxlite
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tmxlite/tmxlite/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Thor
install(TARGETS thor
    EXPORT thorTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT thorTargets
    FILE thorConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/thor
)

install(
    DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Thor/extlibs/aurora/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# sol and nlohmann
install(
    DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/single/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/single_include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# SkellEngine
install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${LIBRARY_NAME}ConfigVersion.cmake
    VERSION ${SkellEngine_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    EXPORT ${LIBRARY_NAME}Targets
    FILE ${LIBRARY_NAME}Targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

configure_file(${LIBRARY_NAME}Config.cmake.in ${LIBRARY_NAME}Config.cmake @ONLY)
install(FILES   "${CMAKE_CURRENT_BINARY_DIR}/SkellEngineConfig.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/SkellEngineConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)

# We export everything
export(
    TARGETS ${LIBRARY_NAME}
            tmxlite
            thor
            lua
    FILE    ${LIBRARY_NAME}Targets.cmake
)

# Packaging support
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "2D C++ Game engine with Lua API")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "STGZ;TGZ;ZIP")

include(CPack)