# This CMake file tries to find TGUI include files and libraries
# For TGUI, see: https://tgui.eu
# TGUI is under Zlib license

# Deduce the libraries suffix from the options
set(FIND_TGUI_LIB_SUFFIX "")
if(SFML_STATIC_LIBRARIES)
    set(FIND_TGUI_LIB_SUFFIX "-s")
endif()

# Find the TGUI include directory
set(FIND_TGUI_PATHS
    ${TGUI_ROOT}
    $ENV{TGUI_ROOT}
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
)
find_path(TGUI_INCLUDE_DIR TGUI/TGUI.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_TGUI_PATHS}
)

# Find the two needed library files
set(FIND_TGUI_LIB_PATHS
    ${TGUI_ROOT}
    $ENV{TGUI_ROOT}
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
    /opt
)

# Debug
find_library(TGUI_LIBRARY_DEBUG
             NAMES tgui${FIND_TGUI_LIB_SUFFIX}-d
             PATH_SUFFIXES lib
             PATHS ${FIND_TGUI_LIB_PATHS}
)

# Release
find_library(TGUI_LIBRARY_RELEASE
             NAMES tgui${FIND_TGUI_LIB_SUFFIX}
             PATH_SUFFIXES lib
             PATHS ${FIND_TGUI_LIB_PATHS}
)

if(TGUI_LIBRARY_RELEASE OR TGUI_LIBRARY_DEBUG)
    # Libraries found
    set(TGUI_FOUND TRUE)

    # If both targets are found, set TGUI_LIBRARY to contain both
    if (TGUI_LIBRARY_RELEASE AND TGUI_LIBRARY_DEBUG)
        set(TGUI_LIBRARY debug ${TGUI_LIBRARY_DEBUG} optimized ${TGUI_LIBRARY_RELEASE})
    endif()
    # If only one target is found, set the other to be equal to the found one
    if (TGUI_LIBRARY_DEBUG AND NOT TGUI_LIBRARY_RELEASE)
        set(TGUI_LIBRARY_RELEASE ${TGUI_LIBRARY_DEBUG})
        set(TGUI_LIBRARY ${TGUI_LIBRARY_DEBUG})
    endif()
    if (NOT TGUI_LIBRARY_DEBUG AND TGUI_LIBRARY_RELEASE)
        set(TGUI_LIBRARY_DEBUG ${TGUI_LIBRARY_RELEASE})
        set(TGUI_LIBRARY ${TGUI_LIBRARY_RELEASE})
    endif()
else()
    # Libraries not found
    set(TGUI_FOUND FALSE)
    set(TGUI_LIBRARY "")
    set(FIND_TGUI_MISSING "${FINDTGUI_MISSING} TGUI_LIBRARY")
endif()

mark_as_advanced(TGUI_INCLUDE_DIR
                 TGUI_LIBRARY
                 TGUI_LIBRARY_DEBUG
                 TGUI_LIBRARY_RELEASE
)

# Result
if(TGUI_FOUND)
    # Success
    message(STATUS "Found TGUI: ${TGUI_INCLUDE_DIR} ${TGUI_LIBRARY}")
else()
    set(FIND_TGUI_ERROR "Could NOT find TGUILite (missing: ${FIND_TGUI_MISSING})")

    if(TGUI_FIND_REQUIRED)
        # Fatal error
        message(FATAL_ERROR ${FIND_TGUI_ERROR})
    elseif(NOT TGUI_FIND_QUIETLY)
        # Error, but continue
        message("${FIND_TGUI_ERROR}")
    endif()
endif()