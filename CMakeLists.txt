cmake_minimum_required(VERSION 3.25)
project(Image_Processing_Program C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

link_directories(${GTK3_LIBRARY_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})

add_executable(Image_Processing_Program main.c imageFunctions.c imageFunctions.h)

target_link_libraries(Image_Processing_Program ${GTK3_LIBRARIES})
