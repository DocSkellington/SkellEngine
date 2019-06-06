Dependencies {#dependencies}
====
[TOC]

# Building {#dependencies_building}
The library files are needed for these dependencies. Please, see the web sites to know how to compile or download these files for your operation system.
  - [Lua](http://www.lua.org/) libraries distributed under [MIT license](https://opensource.org/licenses/mit-license.html). Lua5.2+ is recommended (Lua5.1 has some performance issues and lacks some useful functions).
  - [SFML 2.5](https://www.sfml-dev.org/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).

The following dependencies are given with the source code:
  - A modified version of [tmx-lite](https://github.com/fallahn/tmxlite) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).
  - A modified version of [TGUI](https://tgui.eu/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib). Only the CMakeLists.txt file is modified to avoid interferences with the project configuration. TODO: switch back to the original repository since the CMake script is correct in 0.8.
  - [Thor](http://www.bromeon.ch/libraries/thor/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).
  - [Sol2](https://github.com/ThePhD/sol2) distributed under [MIT license](https://opensource.org/licenses/mit-license.html).
  - [JSON for modern C++](https://github.com/nlohmann/json) distributed under [MIT License](https://opensource.org/licenses/mit-license.html).
  - [Catch2](https://github.com/catchorg/Catch2) distributed under [Boost Software License 1.0](https://opensource.org/licenses/BSL-1.0).

# Documentation {#dependencies_documentation}
The following dependencies are needed to build the documentation:
  - [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) distributed under [GNU General Public License](https://opensource.org/licenses/GPL-2.0).

## Lua documentation
If you want to build the Lua documentation, you will need:
  - Lua executable (see above for links).