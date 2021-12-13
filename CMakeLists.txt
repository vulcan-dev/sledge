# ------------------------------------------------------
# set up cmake
# ------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(sledge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(submodules)

# ------------------------------------------------------
# remove unnecessary windows stuff
# ------------------------------------------------------
string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string (REPLACE "/DWIN32" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

# ------------------------------------------------------
# libraries
# ------------------------------------------------------
add_library(sledge_warnings INTERFACE)
target_compile_options(sledge_warnings INTERFACE /W4 /WX) # /W4 /WX later

# shared dependencies
add_library(sledge_shared_dependencies INTERFACE)
target_link_libraries(sledge_shared_dependencies INTERFACE
	detours
	fmt::fmt
	libglew_static
	imgui)

# globs for shared files
file(GLOB_RECURSE shared_sources CONFIGURE_DEPENDS "source/shared/*.cpp")
file(GLOB_RECURSE shared_headers CONFIGURE_DEPENDS "source/shared/*.h")

# globs for main project
file(GLOB_RECURSE sledge_sources CONFIGURE_DEPENDS "source/sledge/*.cpp")
file(GLOB_RECURSE sledge_headers CONFIGURE_DEPENDS "source/sledge/*.h")


# ------------------------------------------------------
# dll
# ------------------------------------------------------
add_library(sledge SHARED ${sledge_sources} ${shared_sources} ${sledge_headers} ${shared_headers})
target_link_libraries(sledge PRIVATE
    sledge_shared_dependencies
	sledge_warnings)
target_include_directories(sledge PRIVATE "source/sledge" "source/sledge")
target_compile_definitions(sledge PUBLIC _AMD64_ WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS CLIENT_DLL)

# ------------------------------------------------------
# precompiled libs for dll
# ------------------------------------------------------
target_precompile_headers(sledge PRIVATE
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
# loader
# ------------------------------------------------------
file(GLOB_RECURSE sledge_loader_sources CONFIGURE_DEPENDS "source/loader/*.cpp")
file(GLOB_RECURSE sledge_loader_headers CONFIGURE_DEPENDS "source/loader/*.h")

add_executable(sledge_loader WIN32 ${sledge_loader_sources} ${shared_sources} ${sledge_loader_headers} ${shared_headers})
target_link_libraries(sledge_loader PRIVATE
    sledge_shared_dependencies
	sledge_warnings
	glfw
	spng_static)
set_target_properties(sledge_loader PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:sledge_loader>
	OUTPUT_NAME "sledge"
)
target_compile_definitions(sledge_loader PUBLIC _AMD64_ WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
target_include_directories(sledge_loader PRIVATE "source/loader" "source/shared")
add_dependencies(sledge_loader sledge)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sledge_loader)

# ------------------------------------------------------
# merge files from source and include in the ide
# ------------------------------------------------------
function(group_files sources)
	foreach(FILE ${sources})
		get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

		string(REPLACE "/" "\\" GROUP "${GROUP}")

		if("${FILE}" MATCHES "source/loader/.*")
			string(SUBSTRING ${GROUP} 14 -1 GROUP)
		elseif("${FILE}" MATCHES "source/sledge/.*")
			string(SUBSTRING ${GROUP} 14 -1 GROUP)
		elseif("${FILE}" MATCHES "source/shared/.*")
			string(SUBSTRING ${GROUP} 14 -1 GROUP)
		endif()

		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endfunction()
group_files("${sledge_sources};${sledge_loader_sources};${shared_sources};${sledge_headers};${sledge_loader_headers};${shared_headers}")

# ------------------------------------------------------
# install sledge and loader
# ------------------------------------------------------
install(TARGETS
	sledge RUNTIME DESTINATION .)

install(TARGETS
	sledge_loader RUNTIME DESTINATION .)