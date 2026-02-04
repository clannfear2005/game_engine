cmake_minimum_required(VERSION 3.10)

# Prefer GLVND (modern OpenGL)
cmake_policy(SET CMP0072 NEW)

project(game_engine LANGUAGES C CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Makes CMake generate compile_commands.json, which shows the "truth" of the compile lines.
# clangd/clangd-tidy/etc will read this to mirror the real -I/-D flags, like a makefile would.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

# --------------------------------------------------------------------------------------------------
# Finding system libraries
# --------------------------------------------------------------------------------------------------


find_package(OpenGL REQUIRED)

# Provided fall back for if system cannot find the imported target SDL2::SDL2
find_package(SDL2 QUIET)
if (NOT TARGET SDL2::SDL2)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# --------------------------------------------------------------------------------------------------
# Building a static library for glad 
# --------------------------------------------------------------------------------------------------

# Building as a static library creates a .a file (archive file), which functions can then call into.
# This is good because glad is a tiny library, no point in compiling it into separate object files.

# Giving it PUBLIC include directories so anything linking to glad can #include <glad/gl.h>
add_library(glad STATIC lib/glad/src/gl.c)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include
)

# --------------------------------------------------------------------------------------------------
# Building an object library for ImGui
# --------------------------------------------------------------------------------------------------

# Building as an object library to compile sources into .o files, and not archiving them as .a
# Later, the .o files are "inlined" into the final executable.
# This is because

add_library(imgui_objs OBJECT
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/backends/imgui_impl_sdl2.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

# Attaching include dirs to the imgui_objs target.
target_include_directories(imgui_objs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include # Backend needs <glad/gl.h>
)

# Backend must pick the GL loader based on this macro at its compile time
target_compile_definitions(imgui_objs PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD2)

# Ensuring the object library sees SDL2 headers (and propagates useage requirements)
if (TARGET SDL2::SDL2)
    target_link_libraries(imgui_objs PUBLIC SDL2::SDL2)
else()
    target_include_directories(imgui_objs PUBLIC ${SDL2_INCLUDE_DIRS})
endif()

# --------------------------------------------------------------------------------------------------
# Application Target (the executable)
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE PROJECT_SRC CONFIGURE_DEPENDS src/*.cpp)

# $<TARGET_OBJECTS:imgui_objs> is a generator expression, which expands to the .o files produced
# by the object library at build time.

add_executable(game_engine
    ${PROJECT_SRC}
    $<TARGET_OBJECTS:imgui_objs> # inline ImGui objects into the executable
)

# Private include dirs for this target (not globally).
# This means that only game_engine needs these; they don't propagate to its consumers (stuff that uses game_engine)

target_include_directories(game_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include # Project headers
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include    # glad headers (<glad/gl.h>)
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui # ImGui include dirs, incase object useage doesn't propagate 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends 
)

# --------------------------------------------------------------------------------------------------
# Linking (expressing dependencies between targets) 
# --------------------------------------------------------------------------------------------------

# SDL2: Use imported target if available; otherwise wire variables from pkg-config
if (TARGET SDL2::SDL2)
    target_link_libraries(game_engine PRIVATE SDL2::SDL2)

    # Explicitly surfacing include dirs from imported targets so lsp can find <SDL.h>
    target_include_directories(game_engine PRIVATE
        $<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>
    )
else()
    target_include_directories(game_engine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(game_engine PRIVATE ${SDL2_LIBRARIES})
endif()

# Linking OpenGL and GLAD.
target_link_libraries(game_engine PRIVATE OpenGL::GL glad)

# On linux, OpenGL loaders often need libdl and pthread at link time.
if(UNIX AND NOT APPLE)
    target_link_libraries(game_engine PRIVATE dl pthread)
endif()

# The ImGui backend conditionally includes different loader headers based on compile time macro.
# For GLAD 2 (not classic glad), must define for this target.
target_compile_definitions(game_engine PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD2)

# --------------------------------------------------------------------------------------------------
# HOW TO BUILD 
# --------------------------------------------------------------------------------------------------

# 1. Run from project root (where CMakeLists.txt lives)

# 2. configure (generates the build system info ./build)
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# -S = use the current directory as project root,
# -B build = Put all generated files under ./build
# -DCMAKE_BUILD_TYPE=Debug, adds debug info, disables optimisations

# 3. compile
# cmake --build build -j

# -j = build in parallel using all CPU cores.

# 4. Run the program
# ./build/game_engine

# 5. Help clangd see the real compile flags
# ln -sf build/compile_commands.json .
