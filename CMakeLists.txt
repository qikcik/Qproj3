cmake_minimum_required(VERSION 3.25)
project(QProj LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wno-invalid-offsetof -fno-strict-aliasing")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/module.cmake")

find_package(OpenGL REQUIRED)

add_subdirectory("module/3rdParty/stb_image")

add_subdirectory("module/common/log")
add_subdirectory("module/common/template")
add_subdirectory("module/util/json")
add_subdirectory("module/core/typeSystem")
add_subdirectory("module/core/shred")

add_subdirectory("module/library/gui")
add_subdirectory("module/library/openGl")