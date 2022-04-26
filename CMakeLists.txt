# ------------------------------------------------------
# setup cmake 
# ------------------------------------------------------
cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "/EHsc /D_AMD64_ /DWIN32_LEAN_AND_MEAN") # Set C++ exception handler, define platform and ignore some unused headers

set(CMAKE_DOTNET_SDK "Microsoft.NET.Sdk")
set(CMAKE_DOTNET_TARGET_FRAMEWORK, "net6.0")
set(CMAKE_CSharp_FLAGS "/langversion:10.0 /platform:x64")  

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(sledge LANGUAGES CXX CSharp) # enable C++ and C#

add_subdirectory(submodules) # add submodules(fmt, imgui, glfw, etc)

# ------------------------------------------------------
# sledgelib
# ------------------------------------------------------
file(GLOB_RECURSE sledgelib_sources CONFIGURE_DEPENDS "source/sledgelib/*.cs")
add_library(sledgelib SHARED ${sledgelib_sources})

set_target_properties(sledgelib PROPERTIES DOTNET_TARGET_FRAMEWORK "net6.0")
target_compile_options(sledgelib PRIVATE "/langversion:10")

# ------------------------------------------------------
# libraries
# ------------------------------------------------------
add_library(sledge_warnings INTERFACE)
target_compile_options(sledge_warnings INTERFACE /W4 /WX)

# shared dependencies
add_library(sledge_shared_dependencies INTERFACE)
target_link_libraries(sledge_shared_dependencies INTERFACE
	detours
	fmt::fmt
	libglew_static
	glfw)

# globs for shared files
file(GLOB_RECURSE shared_sources CONFIGURE_DEPENDS "source/shared/*.cpp")
file(GLOB_RECURSE shared_headers CONFIGURE_DEPENDS "source/shared/*.h")

# ------------------------------------------------------
# main dll
# ------------------------------------------------------
file(GLOB_RECURSE sledge_sources CONFIGURE_DEPENDS "source/sledge_core/*.cpp")
file(GLOB_RECURSE sledge_headers CONFIGURE_DEPENDS "source/sledge_core/*.h")

add_library(sledge_core SHARED ${sledge_sources} ${shared_sources} ${sledge_headers} ${shared_headers})
target_link_libraries(sledge_core PRIVATE
    sledge_shared_dependencies
	sledgelib
	sledge_warnings
	UltralightCore
	Ultralight
	AppCore
	WebCore
	openvr)
target_include_directories(sledge_core PRIVATE "source/sledge_core" "source/shared")
set_target_properties(sledge_core PROPERTIES OUTPUT_NAME "sledge_core")

# ------------------------------------------------------
# precompiled libs for main dll
# ------------------------------------------------------
target_precompile_headers(sledge_core PRIVATE
    <array>
    <chrono>
    <filesystem>
    <type_traits>
    <fstream>
    <map>
    <memory>
    <regex>
    <string>
    <vector>)

# ------------------------------------------------------
# dumper
# ------------------------------------------------------
file(GLOB_RECURSE dumper_sources CONFIGURE_DEPENDS "source/dumper/*.cpp")
file(GLOB_RECURSE dumper_headers CONFIGURE_DEPENDS "source/dumper/*.h")

add_library(sledge_dumper SHARED ${dumper_sources} ${shared_sources} ${dumper_headers} ${shared_headers})
set_target_properties(sledge_dumper PROPERTIES OUTPUT_NAME "dumper")
target_link_libraries(sledge_dumper PRIVATE
    sledge_shared_dependencies
	sledge_warnings)
target_include_directories(sledge_dumper PRIVATE "source/dumper" "source/shared")

# ------------------------------------------------------
# launcher
# ------------------------------------------------------
file(GLOB_RECURSE sledge_launcher_sources CONFIGURE_DEPENDS "source/launcher/*.cpp")
file(GLOB_RECURSE sledge_launcher_headers CONFIGURE_DEPENDS "source/launcher/*.h")

set(OPENSSL_USE_STATIC_LIBS TRUE) 
find_package(OpenSSL REQUIRED)

add_executable(sledge_launcher WIN32 ${sledge_launcher_sources} ${shared_sources} ${sledge_launcher_headers} ${shared_headers} "resources/launcher_resources.rc")
target_link_libraries(sledge_launcher PRIVATE
    sledge_shared_dependencies
	sledge_warnings
	crypt32
	OpenSSL::Crypto)
set_target_properties(sledge_launcher PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:sledge_launcher>
	OUTPUT_NAME "sledge")
target_include_directories(sledge_launcher PRIVATE "source/launcher" "source/shared")
add_dependencies(sledge_launcher sledge_core)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sledge_launcher)

# ------------------------------------------------------
# merge files from source and include them in the ide
# ------------------------------------------------------
function(group_files sources)
	foreach(FILE ${sources})
		get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" GROUP "${PARENT_DIR}") # remove full dir
		string(REGEX MATCH "[(A-z)|(a-z)|(0-9)]+/[(A-z)|(a-z)|(0-9)]+" GROUPNAME ${GROUP}) # get the first two folders from dir
		
		string(LENGTH ${GROUPNAME} GROUPLEN) # get project name len
		string(SUBSTRING ${GROUP} ${GROUPLEN} -1 GROUP) # remove project name from dir

		source_group("${GROUP}" FILES "${FILE}") # group file
	endforeach()
endfunction()
group_files("${sledgelib_sources};${sledge_sources};${sledge_launcher_sources};${dumper_sources};${shared_sources};${sledge_headers};${sledge_launcher_headers};${dumper_headers};${shared_headers}")

# ------------------------------------------------------
# extra commands
# ------------------------------------------------------

# mod project script
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"${CMAKE_CURRENT_LIST_DIR}\\scripts\\mod.ps1\" \"$(OutDir)mod.ps1\")

# runtime config for hostfxr
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"${CMAKE_CURRENT_LIST_DIR}\\scripts\\sledge.runtimeconfig.json\" \"$(OutDir)bin\\sledge.runtimeconfig.json\")

# ultralight dlls
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"$(ProjectDir)submodules\\deps\\UltraLightCore\\bin\\UltraLightCore.dll\" \"$(OutDir)bin\\UltraLightCore.dll\") 
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"$(ProjectDir)submodules\\deps\\UltraLight\\bin\\UltraLight.dll\" \"$(OutDir)bin\\UltraLight.dll\") 
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"$(ProjectDir)submodules\\deps\\AppCore\\bin\\AppCore.dll\" \"$(OutDir)bin\\AppCore.dll\") 
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"$(ProjectDir)submodules\\deps\\WebCore\\bin\\*\" \"$(OutDir)bin\") 

# ultralight resources
add_custom_command(TARGET sledge_core POST_BUILD COMMAND if not exist \"$(OutDir)ui\" mkdir \"$(OutDir)ui\")
add_custom_command(TARGET sledge_core POST_BUILD COMMAND if not exist \"$(OutDir)ui\\resources\" mkdir \"$(OutDir)ui\\resources\" )
add_custom_command(TARGET sledge_core POST_BUILD COMMAND echo f | xcopy /y /f \"$(ProjectDir)submodules\\deps\\WebCore\\bin\\resources\\*\" \"$(OutDir)ui\\resources\" )

# move sledge_core to bin dir
add_custom_command(TARGET sledge_core POST_BUILD COMMAND move /Y \"$(OutDir)$(TargetName)$(TargetExt)\" \"$(OutDir)bin\\$(TargetName)$(TargetExt)\")
add_custom_command(TARGET sledge_core POST_BUILD COMMAND move /Y \"$(OutDir)$(TargetName).pdb\" \"$(OutDir)bin\\$(TargetName).pdb\")

# move dumper to bin dir
add_custom_command(TARGET sledge_dumper POST_BUILD COMMAND move /Y \"$(OutDir)$(TargetName)$(TargetExt)\" \"$(OutDir)bin\\$(TargetName)$(TargetExt)\")
add_custom_command(TARGET sledge_dumper POST_BUILD COMMAND if exist \"$(OutDir)\\$(TargetName).pdb\" move /Y \"$(OutDir)$(TargetName).pdb\" \"$(OutDir)bin\\$(TargetName).pdb\")

#move openvr binaries to bin folder
add_custom_command(TARGET sledge_core POST_BUILD COMMAND xcopy /y /f \"${CMAKE_CURRENT_LIST_DIR}/submodules/openvr/bin/win64/*\" \"$(OutDir)bin\")

# remove copy caused by target dependency
add_custom_command(TARGET sledge_core POST_BUILD COMMAND del \"$(OutDir)sledgelib.*\" )

# ------------------------------------------------------
# set output directory
# ------------------------------------------------------
set_target_properties(sledge_core sledge_launcher sledge_dumper PROPERTIES RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/build")
set_target_properties(sledgelib PROPERTIES RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/netbuild")