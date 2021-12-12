# ------------------------------------------------------
# remove unnecessary windows stuff from here too
# ------------------------------------------------------
string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string (REPLACE "/DWIN32" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

# ---------------------------
# submodules that already have cmakelists
# ---------------------------
add_subdirectory(fmt)
add_subdirectory(glew-cmake)
add_subdirectory(glm)

# ---------------------------
# detours
# ---------------------------
file(GLOB_RECURSE DETOURS_SOURCES CONFIGURE_DEPENDS "detours/src/*.h" "detours/src/*.cpp")

add_library(lib_detours STATIC ${DETOURS_SOURCES})
set_property(
	SOURCE detours/src/uimports.cpp
	APPEND PROPERTY HEADER_FILE_ONLY true)
target_compile_options(lib_detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
target_include_directories(lib_detours PUBLIC detours/src)