
find_package(Lua QUIET)
if (Lua_FOUND)
    message(STATUS "Lua Found")
    message(STATUS "Lua found: ${LUA_INCLUDE_DIR}")
    message(STATUS "Lua version: ${LUA_VERSION_STRING}")
    message(STATUS "Lua libraries: ${LUA_LIBRARIES}")
    list(APPEND ${LINK_LIBRARIES} ${LUA_LIBRARIES})
else ()

    add_subdirectory(external/lua)
    message(STATUS "Lua version: ${LUA_VERSION}")
    list(APPEND ${LINK_LIBRARIES} lua)
endif ()