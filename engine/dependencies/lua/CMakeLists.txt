set(LUA_SOURCES
    src/lapi.c
    src/lcode.c
    src/lctype.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/llex.c
    src/lmem.c
    src/lobject.c
    src/lopcodes.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/ltable.c
    src/ltm.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcorolib.c
    src/ldblib.c
    src/liolib.c
    src/lmathlib.c
    src/loslib.c
    src/lstrlib.c
    src/ltablib.c
    src/lutf8lib.c
    src/loadlib.c
    src/linit.c
)

set(LUA_HEADERS
    include/lapi.h
    include/lcode.h
    include/ldebug.h
    include/lfunc.h
    include/llex.h
    include/lmem.h
    include/lopcodes.h
    include/lprefix.h
    include/lstring.h
    include/ltm.h
    include/lua.h
    include/lualib.h
    include/lvm.h
    include/lauxlib.h
    include/lctype.h
    include/ldo.h
    include/lgc.h
    include/llimits.h
    include/lobject.h
    include/lparser.h
    include/lstate.h
    include/ltable.h
    include/luaconf.h
    include/lua.hpp
    include/lundump.h
    include/lzio.h
)

add_library(lua SHARED
    ${LUA_SOURCES}
)

target_include_directories(lua
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/lua>
)

install(TARGETS lua
    EXPORT luaConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT luaConfig
    FILE luaConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
)

export(EXPORT luaConfig
    FILE luaConfig.cmake
)