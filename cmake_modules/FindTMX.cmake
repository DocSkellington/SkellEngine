# This CMake file tries to find TMXLite include files and libraries
# For TMXLite, see: https://github.com/fallahn/tmxlite
# TMXLite is under Zlib license

# Deduce the libraries suffix from the options
set(FIND_TMX_LIB_SUFFIX "")
if(SFML_STATIC_LIBRARIES)
    set(FIND_TMX_LIB_SUFFIX "-s")
endif()

# Find the TMXLite include directory
set(FIND_TMX_PATHS
    ${TMX_ROOT}
    $ENV{TMX_ROOT}
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
)
find_path(TMX_INCLUDE_DIR tmxlite/Map.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_TMX_PATHS}
)

# Find the two needed library files
set(FIND_TMX_LIB_PATHS
    ${TMX_ROOT}
    $ENV{TMX_ROOT}
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
    /opt
)

# Debug
find_library(TMX_LIBRARY_DEBUG
             NAMES tmxlite${FIND_TMX_LIB_SUFFIX}-d
             PATH_SUFFIXES lib
             PATHS ${FIND_TMX_LIB_PATHS}
)

# Release
find_library(TMX_LIBRARY_RELEASE
             NAMES tmxlite${FIND_TMX_LIB_SUFFIX}
             PATH_SUFFIXES lib
             PATHS ${FIND_TMX_LIB_PATHS}
)

if(TMX_LIBRARY_RELEASE OR TMX_LIBRARY_DEBUG)
    # Libraries found
    set(TMX_FOUND TRUE)

    # If both targets are found, set TMX_LIBRARY to contain both
    if (TMX_LIBRARY_RELEASE AND TMX_LIBRARY_DEBUG)
        set(TMX_LIBRARY debug ${TMX_LIBRARY_DEBUG} optimized ${TMX_LIBRARY_RELEASE})
    endif()
    # If only one target is found, set the other to be equal to the found one
    if (TMX_LIBRARY_DEBUG AND NOT TMX_LIBRARY_RELEASE)
        set(TMX_LIBRARY_RELEASE ${TMX_LIBRARY_DEBUG})
        set(TMX_LIBRARY ${TMX_LIBRARY_DEBUG})
    endif()
    if (NOT TMX_LIBRARY_DEBUG AND TMX_LIBRARY_RELEASE)
        set(TMX_LIBRARY_DEBUG ${TMX_LIBRARY_RELEASE})
        set(TMX_LIBRARY ${TMX_LIBRARY_RELEASE})
    endif()
else()
    # Libraries not found
    set(TMX_FOUND FALSE)
    set(TMX_LIBRARY "")
    set(FIND_TMX_MISSING "${FINDTMX_MISSING} TMX_LIBRARY")
endif()

mark_as_advanced(TMX_INCLUDE_DIR
                 TMX_LIBRARY
                 TMX_LIBRARY_DEBUG
                 TMX_LIBRARY_RELEASE
)

# Result
if(TMX_FOUND)
    # Success
    message(STATUS "Found TMXLite: ${TMX_INCLUDE_DIR} ${TMX_LIBRARY}")
else()
    set(FIND_TMX_ERROR "Could NOT find TMXLite (missing: ${FIND_TMX_MISSING})")

    if(TMX_FIND_REQUIRED)
        # Fatal error
        message(FATAL_ERROR ${FIND_TMX_ERROR})
    elseif(NOT TMX_FIND_QUIETLY)
        # Error, but continue
        message("${FIND_TMX_ERROR}")
    endif()
endif()