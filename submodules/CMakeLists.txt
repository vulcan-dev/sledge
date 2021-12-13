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

# ---------------------------
# detours
# ---------------------------
file(GLOB_RECURSE DETOURS_SOURCES CONFIGURE_DEPENDS "detours/src/*.h" "detours/src/*.cpp")

add_library(detours STATIC ${DETOURS_SOURCES})
set_property(
	SOURCE detours/src/uimports.cpp
	APPEND PROPERTY HEADER_FILE_ONLY true)
target_compile_options(detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
target_include_directories(detours PUBLIC detours/src)

# ---------------------------
# imgui
# ---------------------------
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS "imgui/*.h" "imgui/*.cpp")
add_library(imgui STATIC
${IMGUI_SOURCES}
imgui/backends/imgui_impl_glfw.cpp
imgui/backends/imgui_impl_glfw.h
imgui/backends/imgui_impl_opengl3.cpp
imgui/backends/imgui_impl_opengl3.h
imgui/backends/imgui_impl_win32.cpp
imgui/backends/imgui_impl_win32.h)
target_compile_options(imgui PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)
target_include_directories(imgui PUBLIC imgui imgui/backends)