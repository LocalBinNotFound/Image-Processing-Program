cmake_minimum_required(VERSION 3.00)
project(Image_Wicked C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(CMAKE_C_STANDARD 11)

link_directories(${GTK3_LIBRARY_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_executable(Image_Wicked mainWindow.c buttonActions.c buttonActions.h)

target_link_libraries(Image_Wicked ${GTK3_LIBRARIES})
