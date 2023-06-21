cmake_minimum_required(VERSION 3.25)
project(Image_Wicked C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

link_directories(${GTK3_LIBRARY_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})

add_executable(Image_Wicked main.c UI.c UI.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)

target_link_libraries(Image_Wicked ${GTK3_LIBRARIES})
