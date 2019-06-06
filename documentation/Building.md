Building the engine {#building}
=====
[TOC]

To build the engine, you will need to perform the following steps:
  - Install the [dependencies](@ref dependencies).
  - Init and download the submodules:

        git submodule init
        git submodule update --recurse

  - Configure the CMake files

        mkdir build
        cd build
        cmake ..
        make install

# CMake configuration
You can configure CMake through the command line or cmake-gui.

  - If you want to build the examples, enable `SKELLENGINE_BUILD_EXAMPLES`.
  - If you want to build the documentation, enable `SKELLENGINE_BUILD_DOC`.
  - If you want to build the tests, enable `SKELLENGINE_BUILD_TESTS`.
  - You can change the installation folder with `CMAKE_INSTALL_PREFIX`.

## Lua documentation
To build the Lua documentation, set `LDOC_EXECUTABLE` to the `ldoc.lua` file.