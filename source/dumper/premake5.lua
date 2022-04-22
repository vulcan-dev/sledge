project "dumper"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    architecture "x64"

    targetdir ("../../build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../../build/Intermediate/%{cfg.buildcfg}/%{prj.name}")
    debugdir("../../build/%{cfg.buildcfg}/%{prj.name}")
    buildoptions { "/EHsc" }
    defines { "/D_AMD64_", "/DWIN32_LEAN_AND_MEAN", "_WINDOWS", "_AMD64_" }

    libdirs {
        "../../build/%{cfg.buildcfg}/detours"
    }

    files {
        "*.h",
        "*.cpp",
        "./shared/*.h",
        "./shared/*.cpp",
    }

    includedirs {
        "../shared/",
        "../../submodules/detours/src"
    }

    links {
        "detours"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        runtime "Release"

    filter {}