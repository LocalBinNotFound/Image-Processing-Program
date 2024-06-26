# Image Wicked
## Description
This program is a simple program that can read, modify, preview, and write an image file of mainstream formats
(JPEG, PNG, BMP, HDR, PSD, TGA, GIF, PIC, PSD, PGM, PPM, ...). It has the following features and functions (more to be added as
development progresses):
1. A GUI: A simple graphical user interface created using GTK to load/preview/modify/save images and navigate in the program
2. Brightness Adjustment: Increase or decrease the brightness of an image.
3. Contrast Adjustment: Increase or decrease the contrast of an image.
4. Gaussian Blur (Soften): Blur an image using a Gaussian filter to smooth out noise and
reduce detail.
5. Laplacian Sharpen (Sharpen): Sharpen an image using Laplacian filter to sharpen details and edges. 6
6. Rotate Image: Rotate an image by a specified angle using interpolation techniques such as
the nearest-neighbor technique.
7. Invert Color: Invert the RGB color of the image.
8. Grayscale: Turn a color image to a grayscale image.
9. Mirror Image: Produce a mirror image of the imported image.
10. RGB: change the red-green-blue (RGB) components of the image.
11. Transparency: change the transparency of an image (image format that supports alpha channel).

## Tools:
- stb image -- single-file public domain libraries for C/C++
  - stb image is an image encoder/decoder library - see: https://github.com/nothings/stb
  - stb supports major image formats JPEG, PNG, BMP, HDR, PSD, TGA, GIF, PIC, PSD, PGM, PPM
  - stb documentation: https://nothings.org/stb/stb_h.html


- GTK (GIMP Toolkit) -- toolkit for creating graphical user interface (GUI)
  - GTK 3.24.38 is deprecated but is the latest old stable GTK for GUI creation -
    see: https://github.com/GNOME/gtk/releases/tag/3.24.38
  - GTK-3.0 documentation https://docs.gtk.org/gtk3/index.html
  - to install GTK 3.24.38: `brew install gtk+3`


- PkgConfig -- package search and retrieval tool for UNIX-like OS
  - to install pkg-config: `brew install pkg-config`


- Cairo - 2D graphics library
  - Cairo is a library that provides a vector graphic-based, device-independent API for developers -
    see: https://cairographics.org/
  - to install cairo: `brew install cairo`
## Program
- to compile:
  ```
  gcc main_window.c image_modifications.c function_buttons.c -o Image_Wicked `pkg-config --cflags --libs gtk+-3.0`
  ```

## Demo
<div style="display: flex; flex-wrap: wrap;">
    <img src="public/original.png" alt="Original" height="300" style="margin-right: 10px;" />
    <img src="public/blurred.png" alt="Blurred" height="300" style="margin-right: 10px;" />
    <img src="public/sharpen.png" alt="Sharpen" height="300" style="margin-right: 10px;" />
    <img src="public/vintage.png" alt="Vintage" height="300" style="margin-right: 10px;"/>
    <img src="public/monochrome.png" alt="Monochrome" height="300" style="margin-right: 10px;"/>
    <img src="public/nostalgic.png" alt="Nostalgic" height="300" style="margin-right: 10px;"/>
</div>