Building the engine {#building}
=====
[TOC]

To build the engine, you will need to perform the following steps:
  - Build and install [SFML](https://www.sfml-dev.org/). If you don't install it in the default folder, you will have to provide `SFML_ROOT` as an environment variable or as a CMake argument.
  - Install Lua (at least 5.3).
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

  - If you want to build the examples, enable `ENGINE_BUILD_EXAMPLES`.
  - If you want to build the documentation, enable `ENGINE_BUILD_DOC`.
  - If you want to build the tests, enable `ENGINE_BUILD_TESTS`.
  - You can change the installation folder with `CMAKE_INSTALL_PREFIX`.