cmake_minimum_required(VERSION 3.25)
project(QProj LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS "-fno-strict-aliasing")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/module.cmake")

add_subdirectory("module/common/log")
