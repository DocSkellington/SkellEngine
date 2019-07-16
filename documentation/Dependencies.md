Dependencies {#dependencies}
====
[TOC]

# Building {#dependencies_building}
The library files are needed for these dependencies. Please, see the web sites to know how to compile or download these files for your operation system.
  - [SFML 2.5](https://www.sfml-dev.org/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).

The following dependencies are given with the source code:
  - A modified version of [tmx-lite](https://github.com/fallahn/tmxlite) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).
  - [TGUI](https://tgui.eu/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).
  - [Thor](http://www.bromeon.ch/libraries/thor/) distributed under [zlib/png license](https://opensource.org/licenses/Zlib).
  - [Sol3](https://github.com/ThePhD/sol2) distributed under [MIT license](https://opensource.org/licenses/mit-license.html).
    - Sol3 proposes a way to compile Lua. To ease Windows development, we use this to build the Lua library. [Lua](http://www.lua.org/) is distributed under [MIT license](https://opensource.org/licenses/mit-license.html). Lua version can be set with the CMake parameter `SOL2_LUA_VERSION`. Lua5.2+ is recommended (Lua5.1 has some performance issues and lacks some useful functions).
  - [JSON for modern C++](https://github.com/nlohmann/json) distributed under [MIT License](https://opensource.org/licenses/mit-license.html).
  - [Catch2](https://github.com/catchorg/Catch2) distributed under [Boost Software License 1.0](https://opensource.org/licenses/BSL-1.0).

# Documentation {#dependencies_documentation}
The following dependencies are needed to build the documentation:
  - [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) distributed under [GNU General Public License](https://opensource.org/licenses/GPL-2.0).

## Lua documentation
If you want to build the Lua documentation, you will need:
  - Lua executable (see above for links).