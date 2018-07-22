# Source files
set(SOURCES
    src/Engine.cpp

    src/entities/Entity.cpp
    src/entities/EntityManager.cpp

    src/entities/components/Component.cpp
    src/entities/components/ExternComponent.cpp
    src/entities/components/GraphicalSpriteSheetComponent.cpp
    src/entities/components/PositionComponent.cpp

    src/files/FileManager.cpp
    src/files/GameDescription.cpp
    src/files/LevelDescription.cpp
    src/files/StateDescription.cpp

    src/map/Layers.cpp
    src/map/Map.cpp

    src/shapes/EllipseShape.cpp

    src/states/GameState.cpp
    src/states/MainMenuState.cpp
    src/states/PauseState.cpp
    src/states/State.cpp
    src/states/StateManager.cpp

    src/systems/GraphicalOrthogonalSystem.cpp
    src/systems/GraphicalSystem.cpp
    src/systems/System.cpp
    src/systems/SystemManager.cpp
    src/systems/ExternSystem.cpp

    src/utilities/json_lua.cpp

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
add_library(${LIBRARY_NAME} SHARED ${SOURCES})

# Includes
target_include_directories(${LIBRARY_NAME} PUBLIC include)
target_include_directories(${LIBRARY_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

# OpenGL
target_link_libraries(${LIBRARY_NAME} GL)

# Detect and add SFML
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${LIBRARY_NAME} sfml-system sfml-graphics sfml-window sfml-audio)
endif()

# Thor
add_subdirectory(dependencies/Thor EXCLUDE_FROM_ALL)
target_include_directories(${LIBRARY_NAME} PUBLIC dependencies/Thor/include dependencies/Thor/extlibs/aurora/include)
target_link_libraries(${LIBRARY_NAME} thor)

# TGUI
add_subdirectory(dependencies/TGUI EXCLUDE_FROM_ALL)
target_include_directories(${LIBRARY_NAME} PUBLIC dependencies/TGUI/include)
target_link_libraries(${LIBRARY_NAME} tgui)

#Lua
find_package(Lua 5.3 REQUIRED)
if (Lua_FOUND)
    target_include_directories(${LIBRARY_NAME} PUBLIC ${LUA_INCLUDE_DIR})
    target_link_libraries(${LIBRARY_NAME} lua5.3)
endif()

# Sol2
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/CMakeLists.txt)
    target_include_directories(${LIBRARY_NAME} PUBLIC dependencies/sol2/single/sol)
else()
    message(FATAL_ERROR "An external dependency (Sol2) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# JSON for modern C++
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/CMakeLists.txt)
    target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/dependencies/json/single_include/nlohmann)
else()
    message(FATAL_ERROR "An external dependency (JSON) could not be found. Don't forget to run 'git submodule init' and 'git submodule update'")
endif()

# std::filesystem for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${LIBRARY_NAME} stdc++fs)
endif()

install(TARGETS ${LIBRARY_NAME}
    LIBRARY DESTINATION .
    ARCHIVE DESTINATION .
)