Building the engine {#building}
=====
[TOC]

To build the engine, you will need to perform the following steps:
  - build and install [SFML](https://www.sfml-dev.org/). If you don't install it in the default folder, you will have to provide `SFML_ROOT` as an environment variable or as a CMake argument.
  - build and install [Thor](http://www.bromeon.ch/libraries/thor/). If you don't install it in the default folder, you will have to provide `THOR_INCLUDE_DIR` and `THOR_LIBRARY_RELEASE` (or `THOR_LIBRARY_DEBUG` if you want to create a Debug build) or simply `THOR_ROOT` as an variable or as a CMake argument.
  - build and install [TMX-Loader](https://github.com/fallahn/sfml-tmxloader)
  - install Lua (at least 5.3)
  - install Python 3.x (to be able to use [JSON for modern C++](https://github.com/nlohmann/json))
  - execute these commands (from the engine root):

        mkdir build && cd build && cmake .. && make install
    
    The dependencies will be checked, sol2 and json will be downloaded and, finally, the game will be build and installed in the `bin` folder.