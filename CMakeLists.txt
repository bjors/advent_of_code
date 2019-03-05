cmake_minimum_required(VERSION 3.9)

project(AdventOfCode VERSION 1.0 LANGUAGES CXX)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB utils_files "utils/*")
add_library(Utils ${utils_files})
target_compile_features(Utils PRIVATE cxx_std_17)
include_directories("${PROJECT_SOURCE_DIR}/utils")
link_libraries(Utils)

file(GLOB solution_files "days/*.cpp")
foreach(file ${solution_files})
    get_filename_component(exe_name ${file} NAME_WE)
    add_executable(${exe_name} ${file})
    target_compile_features(${exe_name} PRIVATE cxx_std_17)
endforeach()
