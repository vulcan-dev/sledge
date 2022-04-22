-- READ
-- I'll be changing where all the solutions and crap are in the future, just want to get this working for now.

workspace "sledge"
    architecture "x64"
    toolset "msc"

    configurations { "Debug", "Release" }
    platforms { "x64" }

-- [[ Format ]] --
project "fmt"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    architecture "x64"

    targetdir ("build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

    compileas "Module"

    files {
        "submodules/fmt/include/fmt/*.h",
        "submodules/fmt/src/*.cc",
    }

    includedirs {
        "submodules/fmt/include",
    }

-- [[ Detours ]] --
project "detours"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    staticruntime "on"

    targetdir ("build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("build/Intermediate/%{cfg.buildcfg}/%{prj.name}")
    buildoptions { "/nologo", "/W4", "/WX", "/we4777", "/we4800", "/Zi", "/MT", "/Gy", "/Gm-", "/Zl", "/Od" }

    files {
        "submodules/detours/src/creatwth.cpp",
        "submodules/detours/src/detours.cpp",
        "submodules/detours/src/detours.h",
        "submodules/detours/src/detver.h",
        "submodules/detours/src/disasm.cpp",
        "submodules/detours/src/disolarm.cpp",
        "submodules/detours/src/disolarm64.cpp",
        "submodules/detours/src/disolia64.cpp",
        "submodules/detours/src/disolx64.cpp",
        "submodules/detours/src/disolx86.cpp",
        "submodules/detours/src/image.cpp",
        "submodules/detours/src/modules.cpp",
    }

    includedirs {
        "submodules/detours/src/",
    }

-- [[ GLFW ]] --
project "glfw" -- https://github.com/HectorPeeters/opengl_premake_boilerplate/blob/master/libs/glfw.lua
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir ("build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

    includedirs { "submodules/glfw/include/" }

    files {
        "submodules/glfw/src/glfw_config.h",
        "submodules/glfw/src/context.c",
        "submodules/glfw/src/init.c",
        "submodules/glfw/src/input.c",
        "submodules/glfw/src/monitor.c",
        "submodules/glfw/src/vulkan.c",
        "submodules/glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files {
            "submodules/glfw/src/win32_init.c",
            "submodules/glfw/src/win32_joystick.c",
            "submodules/glfw/src/win32_monitor.c",
            "submodules/glfw/src/win32_time.c",
            "submodules/glfw/src/win32_thread.c",
            "submodules/glfw/src/win32_window.c",
            "submodules/glfw/src/wgl_context.c",
            "submodules/glfw/src/egl_context.c",
            "submodules/glfw/src/osmesa_context.c"
        }

        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- [[ ImGui ]] --
project "imgui" -- https://github.com/HectorPeeters/opengl_premake_boilerplate/blob/master/libs/imgui.lua
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    includedirs { "submodules/imgui/", "submodules/imgui/examples/", "submodules/glfw/include/" }

    files{
        "submodules/imgui/*.cpp",
        "submodules/imgui/backends/imgui_impl_glfw.cpp",
        "submodules/imgui/backends/imgui_impl_opengl3.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        defines  { 
            "_IMGUI_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- [[ Glew ]] --
project "glew"
    kind "StaticLib"
    language "C"
    architecture "x64"

    targetdir ("build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

    includedirs { "submodules/glew/include/" }

    files {
        "submodules/glew/src/*.c"
    }

include "source/Dumper"