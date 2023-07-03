#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "function_buttons.h"
#include <math.h>

void retrieveOriginalImage();

void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust brightness!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        int brightnessValue = gtk_range_get_value(GTK_RANGE(scale));
        GdkPixbuf *brightPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(brightPixbuf);
        int height = gdk_pixbuf_get_height(brightPixbuf);
        int channels = gdk_pixbuf_get_n_channels(brightPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(brightPixbuf);

        guint8 *startPixel = gdk_pixbuf_get_pixels(brightPixbuf);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8 *pixel = startPixel + y * rowstride + x * channels;

                for (int c = 0; c < channels; c++) {
                    int newValue = pixel[c] + brightnessValue;
                    if (newValue < 0)
                        newValue = 0;
                    else if (newValue > 255)
                        newValue = 255;
                    pixel[c] = newValue;
                }
            }
        }

        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = brightPixbuf;

        updatePreviewBox(previewBoxWithImage);
        g_message("Image brightness adjusted!");
    }
}

void adjustContrast(GtkWidget* button, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust contrast!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        int contrastValue = gtk_range_get_value(GTK_RANGE(button));
        GdkPixbuf* contrastPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(contrastPixbuf);
        int height = gdk_pixbuf_get_height(contrastPixbuf);
        int channels = gdk_pixbuf_get_n_channels(contrastPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(contrastPixbuf);

        guint8* startPixel = gdk_pixbuf_get_pixels(contrastPixbuf);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8* pixel = startPixel + y * rowstride + x * channels;

                for (int c = 0; c < channels; c++) {
                    double pixelValue = pixel[c] / 255.0;
                    double adjustedValue = (pixelValue - 0.5) * contrastValue + 0.5;
                    guint8 newValue = (guint8)(adjustedValue * 255);

                    pixel[c] = newValue;
                }
            }
        }

        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = contrastPixbuf;

        updatePreviewBox(previewBoxWithImage);
        g_message("Image contrast adjusted!");
    }
}

void gaussianBlur(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to blur!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf* blurredPixbuf = gdk_pixbuf_copy(originalPixbuf);
        int width = gdk_pixbuf_get_width(blurredPixbuf);
        int height = gdk_pixbuf_get_height(blurredPixbuf);
        int channels = gdk_pixbuf_get_n_channels(blurredPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(blurredPixbuf);

        guint8* pixels = gdk_pixbuf_get_pixels(blurredPixbuf);

        double sigma = 5.0;  // Adjust the sigma value for desired blurring effect
        int kernelSize = 3;  // Adjust the kernel size for desired blurring effect

        double kernel[kernelSize][kernelSize];
        double sum = 0.0;
        int radius = kernelSize / 2;
        for (int x = -radius; x <= radius; ++x) {
            for (int y = -radius; y <= radius; ++y) {
                double value = exp(-(x * x + y * y) / (2 * sigma * sigma));
                kernel[x + radius][y + radius] = value;
                sum += value;
            }
        }
        for (int i = 0; i < kernelSize; ++i) {
            for (int j = 0; j < kernelSize; ++j) {
                kernel[i][j] /= sum;
            }
        }
        for (int y = radius; y < height - radius; ++y) {
            for (int x = radius; x < width - radius; ++x) {
                for (int c = 0; c < channels; ++c) {
                    double sum = 0.0;
                    for (int i = -radius; i <= radius; ++i) {
                        for (int j = -radius; j <= radius; ++j) {
                            guint8* pixel = pixels + (y + i) * rowstride + (x + j) * channels + c;
                            double value = *pixel * kernel[i + radius][j + radius];
                            sum += value;
                        }
                    }
                    guint8* blurredPixel = pixels + y * rowstride + x * channels + c;
                    *blurredPixel = (guint8)sum;
                }
            }
        }
        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = blurredPixbuf;
        updatePreviewBox(previewBoxWithImage);
        g_message("Image blurred successfully!");
    }
}

void laplacianSharpen(GtkWidget* button, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to apply Laplacian sharpening!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf* sharpenedPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(sharpenedPixbuf);
        int height = gdk_pixbuf_get_height(sharpenedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(sharpenedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(sharpenedPixbuf);

        guint8* pixels = gdk_pixbuf_get_pixels(sharpenedPixbuf);

        double laplacianKernel[3][3] = {
                {-1, -1, -1},
                {-1, 9, -1},
                {-1, -1, -1}
        };

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                for (int c = 0; c < channels; ++c) {
                    double sum = 0.0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            guint8* pixel = pixels + (y + i) * rowstride + (x + j) * channels + c;
                            double value = *pixel * laplacianKernel[i + 1][j + 1];
                            sum += value;
                        }
                    }
                    guint8* sharpenedPixel = pixels + y * rowstride + x * channels + c;
                    *sharpenedPixel = (guint8)fmax(fmin(sum, 255), 0);
                }
            }
        }

        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = sharpenedPixbuf;

        updatePreviewBox(previewBoxWithImage);
        g_message("Image sharpened using Laplacian filter!");
    }
}

void adjustGrayscale(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to convert to grayscale!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf* grayscalePixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(grayscalePixbuf);
        int height = gdk_pixbuf_get_height(grayscalePixbuf);
        int channels = gdk_pixbuf_get_n_channels(grayscalePixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(grayscalePixbuf);

        guint8* pixels = gdk_pixbuf_get_pixels(grayscalePixbuf);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                guint8* pixel = pixels + y * rowstride + x * channels;

                // Calculate grayscale value
                guint8 grayValue = (guint8)((pixel[0] + pixel[1] + pixel[2]) / 3);

                // Set grayscale value for all color channels
                for (int c = 0; c < channels; ++c) {
                    pixel[c] = grayValue;
                }
            }
        }

        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = grayscalePixbuf;

        updatePreviewBox(previewBoxWithImage);
        g_message("Image converted to grayscale!");
    }
}

void rotateByDegree(int degree, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = (PreviewBoxWithImage *) imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to rotate!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;

        // Calculate the radians from the degree
        double radians = degree * (G_PI / 180.0);

        // Compute the new image dimensions after rotation
        int originalWidth = gdk_pixbuf_get_width(originalPixbuf);
        int originalHeight = gdk_pixbuf_get_height(originalPixbuf);
        int rotatedWidth = (int) (fabs(originalWidth * cos(radians)) + fabs(originalHeight * sin(radians)));
        int rotatedHeight = (int) (fabs(originalWidth * sin(radians)) + fabs(originalHeight * cos(radians)));

        // Create a new pixbuf for the rotated image
        GdkPixbuf *rotatedPixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, rotatedWidth, rotatedHeight);
        guchar *pixels = gdk_pixbuf_get_pixels(rotatedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(rotatedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(rotatedPixbuf);

        // Compute the center coordinates of the rotated image
        double centerX = rotatedWidth / 2.0;
        double centerY = rotatedHeight / 2.0;

        // Compute the sine and cosine values of the rotation angle
        double sinTheta = sin(radians);
        double cosTheta = cos(radians);

        // Iterate over each pixel in the rotated image
        for (int y = 0; y < rotatedHeight; ++y) {
            for (int x = 0; x < rotatedWidth; ++x) {
                // Compute the original image coordinates corresponding to the rotated pixel
                double originalX = cosTheta * (x - centerX) + sinTheta * (y - centerY) + originalWidth / 2.0;
                double originalY = -sinTheta * (x - centerX) + cosTheta * (y - centerY) + originalHeight / 2.0;

                // Check if the original image coordinates are within bounds
                if (originalX >= 0 && originalX < originalWidth && originalY >= 0 && originalY < originalHeight) {
                    // Get the interpolated pixel values from the original image
                    int x1 = (int) originalX;
                    int y1 = (int) originalY;
                    int x2 = (x1 + 1 < originalWidth) ? (x1 + 1) : x1;
                    int y2 = (y1 + 1 < originalHeight) ? (y1 + 1) : y1;
                    double alpha = originalX - x1;
                    double beta = originalY - y1;

                    guint8 *pixel1 =
                            gdk_pixbuf_get_pixels(originalPixbuf) + y1 * gdk_pixbuf_get_rowstride(originalPixbuf) +
                            x1 * channels;
                    guint8 *pixel2 =
                            gdk_pixbuf_get_pixels(originalPixbuf) + y1 * gdk_pixbuf_get_rowstride(originalPixbuf) +
                            x2 * channels;
                    guint8 *pixel3 =
                            gdk_pixbuf_get_pixels(originalPixbuf) + y2 * gdk_pixbuf_get_rowstride(originalPixbuf) +
                            x1 * channels;
                    guint8 *pixel4 =
                            gdk_pixbuf_get_pixels(originalPixbuf) + y2 * gdk_pixbuf_get_rowstride(originalPixbuf) +
                            x2 * channels;
                    // Interpolate the pixel values using bilinear interpolation
                    for (int c = 0; c < channels; ++c) {
                        double interpolatedValue = (1 - alpha) * (1 - beta) * pixel1[c] +
                                                   alpha * (1 - beta) * pixel2[c] +
                                                   (1 - alpha) * beta * pixel3[c] +
                                                   alpha * beta * pixel4[c];

                        // Assign the interpolated value to the corresponding pixel in the rotated image
                        pixels[y * rowstride + x * channels + c] = (guchar) interpolatedValue;
                    }
                } else {
                    // Assign black color to out-of-bounds pixels
                    for (int c = 0; c < channels; ++c) {
                        pixels[y * rowstride + x * channels + c] = 0;
                    }
                }
            }
        }

        // Update the originalPixbuf in the imageFile structure with the rotated image
        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = rotatedPixbuf;

        // Update the preview box with the rotated image
        updatePreviewBox(previewBoxWithImage);

        g_message("Image rotated by %d degrees!", degree);
    }
}

    void mirrorImageUpDown() {
}

void mirrorImageLeftRight() {
}


void adjustRGB(GtkWidget *rScale, GtkWidget *gScale, GtkWidget *bScale, GtkWidget *previewBox) {

}

void invertColor(GtkWidget *scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to invert!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf *invertedPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(invertedPixbuf);
        int height = gdk_pixbuf_get_height(invertedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(invertedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(invertedPixbuf);

        guint8 *startPixel = gdk_pixbuf_get_pixels(invertedPixbuf);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8 *pixel = startPixel + y * rowstride + x * channels;
                pixel[0] = 255 - pixel[0];
                pixel[1] = 255 - pixel[1];
                pixel[2] = 255 - pixel[2];
                // pixel[3] is alpha channel
            }
        }
        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = invertedPixbuf;

        updatePreviewBox(previewBoxWithImage);

        g_message("Image inverted successfully!");
    }
}

void adjustTransparency() {

}