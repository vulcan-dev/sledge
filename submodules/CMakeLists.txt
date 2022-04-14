# ---------------------------
# submodules that already have cmakelists
# ---------------------------
add_subdirectory(fmt)
add_subdirectory(glew-cmake)
add_subdirectory(glfw)

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

# ---------------------------
# ultralight
# ---------------------------
set(PLATFORM "win")
set(ARCHITECTURE "x64")

set(ULTRALIGHTCORE_REV "aa5aeef7")
set(ULTRALIGHT_REV "f95886c3")
set(WEBCORE_REV "fcd0b784")
set(APPCORE_REV "a0111c9")

set(ULTRALIGHTCORE_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/UltralightCore/")
set(ULTRALIGHT_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/Ultralight/")
set(WEBCORE_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/WebCore/")
set(APPCORE_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/AppCore/")

set(ULTRALIGHTCORE_BUCKET "ultralightcore-bin")
set(ULTRALIGHT_BUCKET "ultralight-bin")
set(WEBCORE_BUCKET "webcore-bin")
set(APPCORE_BUCKET "appcore-bin")

set(S3_DOMAIN ".sfo2.cdn.digitaloceanspaces.com")

if (NOT EXISTS ${ULTRALIGHTCORE_DIR} OR NOT EXISTS ${ULTRALIGHT_DIR} OR NOT EXISTS ${WEBCORE_DIR} OR NOT EXISTS ${APPCORE_DIR})
    message(STATUS "Downloading Ultralight")
    include(FetchContent)
    FetchContent_Declare(UltralightCoreBin URL https://${ULTRALIGHTCORE_BUCKET}${S3_DOMAIN}/ultralightcore-bin-${ULTRALIGHTCORE_REV}-${PLATFORM}-${ARCHITECTURE}.7z SOURCE_DIR "${ULTRALIGHTCORE_DIR}")
    FetchContent_Declare(UltralightBin URL https://${ULTRALIGHT_BUCKET}${S3_DOMAIN}/ultralight-bin-${ULTRALIGHT_REV}-${PLATFORM}-${ARCHITECTURE}.7z SOURCE_DIR "${ULTRALIGHT_DIR}")
    FetchContent_Declare(WebCoreBin URL https://${WEBCORE_BUCKET}${S3_DOMAIN}/webcore-bin-${WEBCORE_REV}-${PLATFORM}-${ARCHITECTURE}.7z SOURCE_DIR "${WEBCORE_DIR}")
    FetchContent_Declare(AppCoreBin URL https://${APPCORE_BUCKET}${S3_DOMAIN}/appcore-bin-${APPCORE_REV}-${PLATFORM}-${ARCHITECTURE}.7z SOURCE_DIR "${APPCORE_DIR}")
    FetchContent_MakeAvailable(UltralightCoreBin UltralightBin WebCoreBin AppCoreBin)
    message(STATUS "Downloaded Ultralight")
endif()

add_library(UltralightCore SHARED IMPORTED GLOBAL)
set_property(TARGET UltralightCore PROPERTY IMPORTED_IMPLIB "${ULTRALIGHTCORE_DIR}lib/UltralightCore.lib")
target_include_directories(UltralightCore INTERFACE "${ULTRALIGHTCORE_DIR}include")

add_library(Ultralight SHARED IMPORTED GLOBAL)
set_property(TARGET Ultralight PROPERTY IMPORTED_IMPLIB "${ULTRALIGHT_DIR}lib/Ultralight.lib")
target_include_directories(Ultralight INTERFACE "${ULTRALIGHT_DIR}include")

add_library(WebCore SHARED IMPORTED GLOBAL)
set_property(TARGET WebCore PROPERTY IMPORTED_IMPLIB "${WEBCORE_DIR}lib/WebCore.lib")
target_include_directories(WebCore INTERFACE "${WEBCORE_DIR}include")

add_library(AppCore SHARED IMPORTED GLOBAL)
set_property(TARGET AppCore PROPERTY IMPORTED_IMPLIB "${APPCORE_DIR}lib/AppCore.lib")
target_include_directories(AppCore INTERFACE "${APPCORE_DIR}include")