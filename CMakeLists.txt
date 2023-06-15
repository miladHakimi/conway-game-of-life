cmake_minimum_required(VERSION 3.15)

# #project name
project(game VERSION 1.0)

add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_17)
target_include_directories(tutorial_compiler_flags INTERFACE .)
set (CMAKE_CXX_FLAGS "-std=c++17")

find_package(SDL2 REQUIRED)
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR})
add_executable(game main.cpp)
target_link_libraries(game ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY})
