include(ExternalProject)

set(TMX_VERSION 2.0.0)

set(TMX_INSTALL ${CMAKE_BINARY_DIR}/tmx)

set(VENDOR_PATH ${CMAKE_BINARY_DIR}/dependencies/tmx)
ExternalProject_add(
    tmx
    PREFIX ${VENDOR_PATH}

    GIT_REPOSITORY
    "https://github.com/fallahn/sfml-tmxloader.git"
    GIT_TAG ${TMX_VERSION}

    CMAKE_ARGS -DTMX_BUILD_EXAMPLE=FALSE -DCMAKE_INSTALL_PREFIX=${TMX_INSTALL}

    TEST_COMMAND ""
)

include_directories(${TMX_INSTALL}/include)
link_directories(${TMX_INSTALL}/lib)