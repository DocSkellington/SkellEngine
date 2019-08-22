---
layout: page
title: How to build and dependencies
---

First, we need to download the necessary dependencies. After that, we can compile and use SkellEngine.

# Dependencies
## What to install
  - CMake (for Windows and inexperienced users, the `cmake-gui` program is recommended).
  - A C++ compiler compatible with C++17 standard.
  - [SFML](https://www.sfml-dev.org/) at least version 2.5.

If you want to build the documentation, you also need:
  - [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) distributed under the [GNU General Public License version 2](https://opensource.org/licenses/GPL-2.0).
  - A Lua executable (see below for more information on Lua).

## Dependencies given with the source code
For your curiosity, here is the full list of dependencies that are already given with the source code:
  - [tmxlite](https://github.com/fallahn/tmxlite) distributed under the [zlib/png license](https://opensource.org/licenses/Zlib).
  - [TGUI](https://tgui.eu) distributed under the [zlib/png license](https://opensource.org/licenses/Zlib).
  - [Thor](http://www.bromeon.ch/libraries/thor/) distributed under the [zlib/png license](https://opensource.org/licenses/Zlib).
  - [Lua 5.3.5](https://www.lua.org/) distributed under the [MIT license](https://opensource.org/licenses/mit-license.html). The `lua` executable is **NOT** built.
  - [sol3](https://github.com/ThePhD/sol2) distributed under the [MIT license](https://opensource.org/licenses/mit-license.html).
  - [JSON for modern C++](https://github.com/nlohmann/json) distributed under the [MIT license](https://opensource.org/licenses/mit-license.html).

For the unit tests, [Catch2](https://github.com/catchorg/Catch2) (distributed under the [Boost Software License 1.0](https://opensource.org/licenses/BSL-1.0)) is also given.

# How to build
  1. Download the source code from the [GitHub repository]({{ site.github.repository_url }}). If you cloned the repository, do not forget to download the submodules.
  2. In the root directory of the project, create a folder named `build`.
  3. Configure CMake in the `build` directory (if you do not know how to do it, this [SFML tutorial](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) might help you to get started).
  4. Build and install the project (using `make` or an IDE project depending on your CMake configuration).

## Command line
Under GNU/Linux distributions, use the following command line:
```
git clone --recurse-submodules {{ site.github.repository_url }} && \
cd SkellEngine && \
mkdir build && \
cd build && \
cmake .. && \
make
```

# Use SkellEngine in your project
Since SkellEngine is built using CMake, it's quite easy to set SkellEngine as a library in your CMake project. In your CMakeLists.txt, add the following line:

    find_package(SkellEngine CONFIG REQUIRED)

This adds a library called `SkellEngine`. You can use it as follow:

    add_executable(YourExecutable ${Your_Files})
    target_link_libraries(YourExecutable PUBLIC SkellEngine)

Full examples can be seen in the other tutorials.