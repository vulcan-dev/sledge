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
add_subdirectory(glfw)

set(BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
add_subdirectory(miniz)

# ---------------------------
# detours
# ---------------------------
file(GLOB_RECURSE detours_sources CONFIGURE_DEPENDS "detours/src/*.h" "detours/src/*.cpp")

add_library(detours STATIC ${detours_sources})
set_property(
	SOURCE detours/src/uimports.cpp
	APPEND PROPERTY HEADER_FILE_ONLY true)
target_compile_options(detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
target_include_directories(detours PUBLIC detours/src)

# ---------------------------
# steamworks
# ---------------------------
set(STEAMWORKS_LIBRARY_SUFFIX "64")
set(BUILD_ARCH "64")
SET(STEAMWORKS_LIBRARY_SUFFIX "64")
set(PLATFORM "win")

find_path(STEAMWORKS_INCLUDE_DIR
	NAMES steam/steam_api.h
	PATHS steamworks/public)

set(STEAMWORKS_PATH_SUFFIXES ${STEAMWORKS_PATH_SUFFIXES} ${PLATFORM}${BUILD_ARCH}/)

find_library(STEAMWORKS_LIBRARY
	NAMES steam_api${STEAMWORKS_LIBRARY_SUFFIX}
	PATH_SUFFIXES ${STEAMWORKS_PATH_SUFFIXES}
	PATHS steamworks/redistributable_bin/)

find_library(STEAMWORKS_APP_TICKET_LIBRARY
	NAMES sdkencryptedappticket sdkencryptedappticket64
	PATH_SUFFIXES ${STEAMWORKS_PATH_SUFFIXES}
	PATHS steamworks/public/steam/lib/)

add_library(steamworks_common STATIC IMPORTED)
set_target_properties(steamworks_common PROPERTIES
	IMPORTED_LOCATION ${STEAMWORKS_LIBRARY}
	INTERFACE_INCLUDE_DIRECTORIES ${STEAMWORKS_INCLUDE_DIR})

add_library(steamworks_appticket STATIC IMPORTED)
set_target_properties(steamworks_appticket PROPERTIES
	IMPORTED_LOCATION ${STEAMWORKS_APP_TICKET_LIBRARY}
	INTERFACE_INCLUDE_DIRECTORIES ${STEAMWORKS_INCLUDE_DIR})

add_library(steamworks INTERFACE IMPORTED GLOBAL)
set_property(TARGET steamworks PROPERTY
	INTERFACE_LINK_LIBRARIES steamworks_common steamworks_appticket)
target_include_directories(steamworks INTERFACE steamworks/public)

# ---------------------------
# spng (already has a cmakelists but it can't be made so it uses miniz rather than zlib)
# ---------------------------
add_library(spng_static STATIC "libspng/spng/spng.c")
target_compile_options(spng_static PUBLIC -DSPNG_STATIC=1 -DSPNG_USE_MINIZ=1)
target_include_directories(spng_static PUBLIC "libspng/spng")
target_link_libraries(spng_static PRIVATE miniz)

# ---------------------------
# imgui
# ---------------------------
file(GLOB imgui_sources CONFIGURE_DEPENDS "imgui/*.h" "imgui/*.cpp")
add_library(imgui STATIC
${imgui_sources}
imgui/backends/imgui_impl_glfw.cpp
imgui/backends/imgui_impl_glfw.h
imgui/backends/imgui_impl_opengl3.cpp
imgui/backends/imgui_impl_opengl3.h
imgui/backends/imgui_impl_win32.cpp
imgui/backends/imgui_impl_win32.h)
target_compile_options(imgui PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_link_libraries(imgui PRIVATE glfw)