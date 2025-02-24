# Copied from http://sol2.readthedocs.io/en/latest/cmake.html
# Needed for ExternalProject_Add()
include(ExternalProject)

# Needed for building single header for sol2
find_package(PythonInterp 3 REQUIRED)

# Configuration data for What sol2 version to use and where to put it
set(SOL2_TAG v2.5.6)
set(SOL2_HPP "${CMAKE_BINARY_DIR}/include/sol.hpp")

set(VENDOR_PATH ${CMAKE_BINARY_DIR}/dependencies/Sol2)
# Download and "install" sol2
ExternalProject_add(
    sol2
    PREFIX ${VENDOR_PATH} 

    GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
    GIT_TAG ${SOL2_TAG}

    # No CMake commands to run, so tell CMake not to configure
    CONFIGURE_COMMAND ""

    # Generate the single header and put it in ${SOL2_HPP}
    BINARY_DIR ${VENDOR_PATH}/src/sol2
    BUILD_COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include
        COMMAND
            ${PYTHON_EXECUTABLE} ./single.py -o "${SOL2_HPP}"

    # No install or test command for the library
    INSTALL_COMMAND ""
    TEST_COMMAND "")

# Conditionally turn on SOL_CHECK_ARGUMENTS if using Debug mode
if (CMAKE_BUILD_TYPE MATCHES "[Dd]ebug")
    if (VERBOSE)
        message(STATUS "Turning on SOL_CHECK_ARGUMENTS in Debug mode.")
    endif()
    add_definitions(-DSOL_CHECK_ARGUMENTS)
endif()

# Make sure sol2 is found as a system directory
include_directories(${CMAKE_BINARY_DIR}/include)