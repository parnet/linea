cmake_minimum_required(VERSION 3.10)
project(lua LANGUAGES CXX)

# Set the version of Lua you are building
set(LUA_VERSION "5.4.7")

set(CMAKE_BUILD_TYPE Release)  #

# Specify the source files for Lua
set(LUA_SRC
        lua/src/lapi.c
        lua/src/lauxlib.c
        lua/src/lbaselib.c
        lua/src/lcode.c
        lua/src/ldblib.c
        lua/src/ldebug.c
        lua/src/ldo.c
        lua/src/ldump.c
        lua/src/lfunc.c
        lua/src/lgc.c
        lua/src/linit.c
        lua/src/liolib.c
        lua/src/llex.c
        lua/src/lmathlib.c
        lua/src/lmem.c
        lua/src/loadlib.c
        lua/src/lobject.c
        lua/src/lopcodes.c
        lua/src/loslib.c
        lua/src/lparser.c
        lua/src/lstate.c
        lua/src/lstring.c
        lua/src/lstrlib.c
        lua/src/ltable.c
        lua/src/ltablib.c
        lua/src/ltm.c
        lua/src/lua.c
        lua/src/luac.c
        lua/src/lundump.c
        lua/src/lutf8lib.c
        lua/src/lvm.c
        lua/src/lzio.c
        lua/src/lcorolib.c
        lua/src/lctype.c
)

foreach(source_file ${LUA_SRC})
    set_source_files_properties(${source_file} PROPERTIES LANGUAGE CXX)
endforeach()

# Create a static library for Lua
add_library(lua STATIC ${LUA_SRC})
add_definitions(-DLUA_USE_POSIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3")

# Include the Lua headers
target_include_directories(lua PUBLIC lua/src)

# Optionally create an executable for the Lua interpreter
# add_executable(lua_interpreter lua/src/lua.c)
# set_source_files_properties(src/lua.c PROPERTIES LANGUAGE CXX)
# target_link_libraries(lua_interpreter PRIVATE lua)

# Optionally create an executable for the Lua bytecode compiler (luac)
# add_executable(luac lua/src/luac.c)
# set_source_files_properties(lua/src/luac.c PROPERTIES LANGUAGE CXX)
# target_link_libraries(luac PRIVATE lua)