cmake_minimum_required(VERSION 3.00)
project(Image_Wicked C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(ImageMagick REQUIRED IMPORTED_TARGET MagickWand)

set(CMAKE_C_STANDARD 11)

link_directories(${GTK3_LIBRARY_DIRS})
include_directories(${GTK3_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_executable(Image_Wicked main_window.c function_buttons.c function_buttons.h image_modifications.c image_modifications.h)

target_link_libraries(Image_Wicked ${GTK3_LIBRARIES} m PkgConfig::ImageMagick)
