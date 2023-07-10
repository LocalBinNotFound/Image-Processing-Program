#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "function_buttons.h"
#include "image_modifications.h"
#include <math.h>

const double minPixelValue = 0.0;
const double maxPixelValue = 255.0;

void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust brightness!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf *pixbufAdjustedByFunc = previewBoxWithImage->pixbufAdjustedByFunc;
        double previousScaleValue = previewBoxWithImage->prevBrightnessScaleValue;

        double brightnessValue = gtk_range_get_value(GTK_RANGE(scale));

        int width = gdk_pixbuf_get_width(originalPixbuf);
        int height = gdk_pixbuf_get_height(originalPixbuf);
        int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
        guint8 *startPixel = gdk_pixbuf_get_pixels(originalPixbuf);
        guint8 *adjustedStartPixel = gdk_pixbuf_get_pixels(pixbufAdjustedByFunc);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8 *pixel = startPixel + y * rowstride + x * channels;
                guint8 *originalPixel = adjustedStartPixel + y * rowstride + x * channels;
                for (int c = 0; c < 3; c++) {
                    double leftBound = originalPixel[c] * -1;
                    double rightBound = 255 - originalPixel[c];
                    double newValue;
                    if (brightnessValue <= rightBound && brightnessValue >= leftBound) {
                        double startPoint;
                        if (previousScaleValue < leftBound) {
                            startPoint = leftBound;
                        } else if (previousScaleValue > rightBound) {
                            startPoint = rightBound;
                        } else {
                            startPoint = previousScaleValue;
                        }
                        double brightnessChange = brightnessValue - startPoint;
                        newValue = pixel[c] + brightnessChange;
                    } else if (brightnessValue > rightBound) {
                        newValue = maxPixelValue;
                    } else {
                        newValue = minPixelValue;
                    }
                    pixel[c] = (guint8)newValue;
                }
            }
        }
        updatePreviewBox(previewBoxWithImage);
        previewBoxWithImage->prevBrightnessScaleValue = brightnessValue;
    }
}

void adjustContrast(GtkWidget* scale, gpointer imageFile) {
    static GdkPixbuf *resetToOriginalPixbuf = NULL;
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust contrast!");
    } else {
        double contrastValue = gtk_range_get_value(GTK_RANGE(scale));
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        double previousScaleValue = previewBoxWithImage->prevContrastScaleValue;
        double contrastChange = contrastValue - previousScaleValue;

        if (contrastValue == 0.0) {
            if (resetToOriginalPixbuf != NULL) {
                g_object_unref(previewBoxWithImage->originalPixbuf);
                previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(resetToOriginalPixbuf);
                updatePreviewBox(previewBoxWithImage);
            } else {
                g_message("No image available to reset contrast!");
            }
        } else {
            if (resetToOriginalPixbuf == NULL) {
                resetToOriginalPixbuf = gdk_pixbuf_copy(previewBoxWithImage->originalPixbuf);
            }
            int width = gdk_pixbuf_get_width(originalPixbuf);
            int height = gdk_pixbuf_get_height(originalPixbuf);
            int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
            int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
            guint8 *pixels = gdk_pixbuf_get_pixels(originalPixbuf);
            guint8 rgbThreshold = 127;
            double contrast = contrastChange / 100.0;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    guint8 *pixel = pixels + y * rowstride + x * channels;

                    for (int c = 0; c < channels; c++) {
                        double pixelValue = pixel[c];
                        double adjustedValue = pixelValue + (pixelValue - rgbThreshold) * contrast;
                        if (adjustedValue < 0)
                            adjustedValue = 0;
                        else if (adjustedValue > 255)
                            adjustedValue = 255;
                        pixel[c] = (guint8) adjustedValue;
                    }
                }
            }
            updatePreviewBox(previewBoxWithImage);
            g_object_unref(previewBoxWithImage->pixbufAdjustedByFunc);
            previewBoxWithImage->pixbufAdjustedByFunc = gdk_pixbuf_copy(originalPixbuf);
            previewBoxWithImage->prevContrastScaleValue = contrastValue;
        }
    }
}


void gaussianBlur(GtkWidget* button, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

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

        const gchar* sigmaStr = gtk_entry_get_text(GTK_ENTRY(previewBoxWithImage->sigmaEntry));
        double sigma = g_ascii_strtod(sigmaStr, NULL);
        int kernelSize = previewBoxWithImage->softenKernelData;

        double gaussianKernel[kernelSize][kernelSize];
        double sum = 0.0;
        int radius = kernelSize / 2;
        for (int x = -radius; x <= radius; ++x) {
            for (int y = -radius; y <= radius; ++y) {
                double value = exp(-(x * x + y * y) / (2 * sigma * sigma));
                gaussianKernel[x + radius][y + radius] = value;
                sum += value;
            }
        }
        for (int i = 0; i < kernelSize; ++i) {
            for (int j = 0; j < kernelSize; ++j) {
                gaussianKernel[i][j] /= sum;
            }
        }
        for (int y = radius; y < height - radius; ++y) {
            for (int x = radius; x < width - radius; ++x) {
                for (int c = 0; c < channels; ++c) {
                    double sum = 0.0;
                    for (int i = -radius; i <= radius; ++i) {
                        for (int j = -radius; j <= radius; ++j) {
                            guint8* pixel = pixels + (y + i) * rowstride + (x + j) * channels + c;
                            double value = *pixel * gaussianKernel[i + radius][j + radius];
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
    static double previousScaleValue = 0.0;
    static GdkPixbuf *originalPixbuf = NULL;  // Keep track of the original image
    PreviewBoxWithImage *previewBoxWithImage = (PreviewBoxWithImage *) imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to apply Laplacian sharpening!");
        return;
    }

    double sharpenScale = gtk_range_get_value(GTK_RANGE(button));
    double sharpenFactor = pow(10, sharpenScale) * 0.1;  // Adjust the sharpening factor based on the scale

    if (sharpenScale == previousScaleValue) {
        // Undo the sharpening
        if (originalPixbuf != NULL) {
            g_object_unref(previewBoxWithImage->originalPixbuf);
            previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(originalPixbuf);
            updatePreviewBox(previewBoxWithImage);
            g_message("Laplacian sharpening undone!");
            previousScaleValue = 0.0;  // Reset the previous scale value
        }
    } else {
        // Store the original image
        if (originalPixbuf == NULL) {
            originalPixbuf = gdk_pixbuf_copy(previewBoxWithImage->originalPixbuf);
        }

        GdkPixbuf *sharpenedPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(sharpenedPixbuf);
        int height = gdk_pixbuf_get_height(sharpenedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(sharpenedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(sharpenedPixbuf);
        guint8 *pixels = gdk_pixbuf_get_pixels(sharpenedPixbuf);

        // Apply the Laplacian sharpening filter
        double laplacianKernel[3][3] = {
                {-1, -1, -1},
                {-1, 9,  -1},
                {-1, -1, -1}
        };

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                for (int c = 0; c < channels; ++c) {
                    double sum = 0.0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            guint8 *pixel = pixels + (y + i) * rowstride + (x + j) * channels + c;
                            double value = *pixel * laplacianKernel[i + 1][j + 1];
                            sum += value;
                        }
                    }
                    guint8 *sharpenedPixel = pixels + y * rowstride + x * channels + c;
                    *sharpenedPixel = (guint8) fmin(fmax(*sharpenedPixel + sharpenFactor * sum, 0), 255);
                }
            }
        }
        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = sharpenedPixbuf;
        updatePreviewBox(previewBoxWithImage);
        g_message("Image sharpened using Laplacian filter!");
    }
}


void turnIntoGrayscale(GtkWidget* button, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to convert to grayscale!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        int width = gdk_pixbuf_get_width(originalPixbuf);
        int height = gdk_pixbuf_get_height(originalPixbuf);
        int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
        guint8* pixels = gdk_pixbuf_get_pixels(originalPixbuf);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                guint8* pixel = pixels + y * rowstride + x * channels;
                guint8 grayValue = (guint8)((pixel[0] + pixel[1] + pixel[2]) / 3);
                for (int c = 0; c < channels; ++c) {
                    pixel[c] = grayValue;
                }
            }
        }
        updatePreviewBox(previewBoxWithImage);
    }
}


void adjustGrayscale(GtkWidget* scale, gpointer imageFile) {

}


// Rotate an image by a specified angle using interpolation techniques such as nearest-neighbor technique
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

PreviewBoxWithImage* getPreviewBoxImage(GtkWidget* previewBox) {
    // Cast the widget to a GtkImage
    GtkImage* image = GTK_IMAGE(previewBox);

    // Get the Pixbuf from the GtkImage
    GdkPixbuf* pixbuf = gtk_image_get_pixbuf(image);

    // Create a new PreviewBoxWithImage structure
    PreviewBoxWithImage* previewBoxWithImage = malloc(sizeof(PreviewBoxWithImage));
    previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(pixbuf);

    return previewBoxWithImage;
}

// need to implement
void mirrorImageUpDown(GtkWidget* scale, gpointer imageFile) {
    // Retrieve the original image from the preview box
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to mirror!");
        return;
    }

    // Retrieve the original pixbuf and its properties
    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);

    // Create a new pixbuf for the mirrored image
    GdkPixbuf* mirroredPixbuf = gdk_pixbuf_copy(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(mirroredPixbuf);

    // Iterate over each row of pixels and mirror them vertically
    for (int y = 0; y < height / 2; ++y) {
        // Compute the corresponding row index for mirroring
        int mirroredY = height - y - 1;

        // Swap the pixel data between the original row and the mirrored row
        guint8* originalRow = pixels + y * rowstride;
        guint8* mirroredRow = pixels + mirroredY * rowstride;
        for (int x = 0; x < width * channels; ++x) {
            guint8 temp = originalRow[x];
            originalRow[x] = mirroredRow[x];
            mirroredRow[x] = temp;
        }
    }

    // Update the originalPixbuf in the previewBoxWithImage structure with the mirrored image
    g_object_unref(previewBoxWithImage->originalPixbuf);
    previewBoxWithImage->originalPixbuf = mirroredPixbuf;

    // Update the preview box with the mirrored image
    updatePreviewBox(previewBoxWithImage);

    g_message("Image mirrored vertically!");
}


void mirrorImageLeftRight(GtkWidget* scale, gpointer imageFile) {// Retrieve the original image from the preview box
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to mirror!");
        return;
    }

    // Retrieve the original pixbuf and its properties
    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);

    // Create a new pixbuf for the mirrored image
    GdkPixbuf* mirroredPixbuf = gdk_pixbuf_copy(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(mirroredPixbuf);

    // Iterate over each row of pixels and mirror them horizontally
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width / 2; ++x) {
            // Compute the corresponding column index for mirroring
            int mirroredX = width - x - 1;

            // Swap the pixel data between the original column and the mirrored column
            guint8* originalPixel = pixels + y * rowstride + x * channels;
            guint8* mirroredPixel = pixels + y * rowstride + mirroredX * channels;

            for (int c = 0; c < channels; ++c) {
                guint8 temp = originalPixel[c];
                originalPixel[c] = mirroredPixel[c];
                mirroredPixel[c] = temp;
            }
        }
    }

    // Update the originalPixbuf in the previewBoxWithImage structure with the mirrored image
    g_object_unref(previewBoxWithImage->originalPixbuf);
    previewBoxWithImage->originalPixbuf = mirroredPixbuf;

    // Update the preview box with the mirrored image
    updatePreviewBox(previewBoxWithImage);

    g_message("Image mirrored horizontally!");
}

void adjustR(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* adjustedPixbuf = previewBoxWithImage->pixbufAdjustedByFunc;
    double previousR = previewBoxWithImage->prevR;

    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(originalPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(adjustedPixbuf);


    // Get the current values from the RGB scale bars
    double redValue = gtk_range_get_value(GTK_RANGE(scale));

    // Adjust the RGB values based on the scales
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8* pixel = pixels + y * rowstride + x * channels;
            guint8* tempPixel = tempPixels + y * rowstride + x * channels;
            double leftBound = tempPixel[0] * -1;
            double rightBound = 255 - tempPixel[0];
            double newValue;
            if (redValue <= rightBound && redValue >= leftBound) {
                double startPoint;
                if (previousR < leftBound) {
                    startPoint = leftBound;
                } else if (previousR > rightBound) {
                    startPoint = rightBound;
                } else {
                    startPoint = previousR;
                }
                double rChange = redValue - startPoint;
                newValue = pixel[0] + rChange;
            } else if (redValue > rightBound) {
                newValue = maxPixelValue;
            } else {
                newValue = minPixelValue;
            }
            pixel[0] = (guint8)newValue;
        }
    }

    // Update the previewBoxWithImage structure
    previewBoxWithImage->prevR = redValue;
    updatePreviewBox(previewBoxWithImage);

    g_message("Image RGB adjusted!");
}

void adjustG(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* adjustedPixbuf = previewBoxWithImage->pixbufAdjustedByFunc;
    double previousG = previewBoxWithImage->prevG;

    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(originalPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(adjustedPixbuf);

    // Get the current values from the RGB scale bars
    double greenValue = gtk_range_get_value(GTK_RANGE(scale));

    // Adjust the RGB values based on the scales
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8* pixel = pixels + y * rowstride + x * channels;
            guint8* tempPixel = tempPixels + y * rowstride + x * channels;
            double leftBound = tempPixel[1] * -1;
            double rightBound = 255 - tempPixel[1];
            double newValue;
            if (greenValue <= rightBound && greenValue >= leftBound) {
                double startPoint;
                if (previousG < leftBound) {
                    startPoint = leftBound;
                } else if (previousG > rightBound) {
                    startPoint = rightBound;
                } else {
                    startPoint = previousG;
                }
                double rChange = greenValue - startPoint;
                newValue = pixel[1] + rChange;
            } else if (greenValue > rightBound) {
                newValue = maxPixelValue;
            } else {
                newValue = minPixelValue;
            }
            pixel[1] = (guint8)newValue;
        }
    }

    // Update the previewBoxWithImage structure
    previewBoxWithImage->prevG = greenValue;
    updatePreviewBox(previewBoxWithImage);

    g_message("Image RGB adjusted!");
}

void adjustB(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* adjustedPixbuf = previewBoxWithImage->pixbufAdjustedByFunc;
    double previousB = previewBoxWithImage->prevB;

    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(originalPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(adjustedPixbuf);


    // Get the current values from the RGB scale bars
    double blueValue = gtk_range_get_value(GTK_RANGE(scale));

    // Adjust the RGB values based on the scales
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8* pixel = pixels + y * rowstride + x * channels;
            guint8* tempPixel = tempPixels + y * rowstride + x * channels;
            double leftBound = tempPixel[2] * -1;
            double rightBound = 255 - tempPixel[2];
            double newValue;
            if (blueValue <= rightBound && blueValue >= leftBound) {
                double startPoint;
                if (previousB < leftBound) {
                    startPoint = leftBound;
                } else if (previousB > rightBound) {
                    startPoint = rightBound;
                } else {
                    startPoint = previousB;
                }
                double rChange = blueValue - startPoint;
                newValue = pixel[2] + rChange;
            } else if (blueValue > rightBound) {
                newValue = maxPixelValue;
            } else {
                newValue = minPixelValue;
            }
            pixel[2] = (guint8)newValue;
        }
    }

    // Update the previewBoxWithImage structure
    previewBoxWithImage->prevB = blueValue;
    updatePreviewBox(previewBoxWithImage);

    g_message("Image RGB adjusted!");
}


void adjustRGB(GtkWidget* redScale, GtkWidget* greenScale, GtkWidget* blueScale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* adjustedPixbuf = gdk_pixbuf_copy(originalPixbuf);
    GdkPixbuf* tempPixbuf = gdk_pixbuf_copy(originalPixbuf);

    int width = gdk_pixbuf_get_width(adjustedPixbuf);
    int height = gdk_pixbuf_get_height(adjustedPixbuf);
    int channels = gdk_pixbuf_get_n_channels(adjustedPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(adjustedPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(adjustedPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(tempPixbuf);

    double maxRed = 0.0;
    double maxGreen = 0.0;
    double maxBlue = 0.0;

    // Find the maximum RGB values in the original image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8* pixel = pixels + y * rowstride + x * channels;
            if (pixel[0] > maxRed)
                maxRed = pixel[0];
            if (pixel[1] > maxGreen)
                maxGreen = pixel[1];
            if (pixel[2] > maxBlue)
                maxBlue = pixel[2];
        }
    }

    // Set the maximum values on the RGB scale bars
    GtkRange* redRange = GTK_RANGE(redScale);
    GtkRange* greenRange = GTK_RANGE(greenScale);
    GtkRange* blueRange = GTK_RANGE(blueScale);

    gtk_range_set_adjustment(redRange, gtk_adjustment_new(0.0, 0.0, maxRed, 1.0, 1.0, 0.0));
    gtk_range_set_adjustment(greenRange, gtk_adjustment_new(0.0, 0.0, maxGreen, 1.0, 1.0, 0.0));
    gtk_range_set_adjustment(blueRange, gtk_adjustment_new(0.0, 0.0, maxBlue, 1.0, 1.0, 0.0));

    // Get the current values from the RGB scale bars
    double redValue = gtk_range_get_value(redRange);
    double greenValue = gtk_range_get_value(greenRange);
    double blueValue = gtk_range_get_value(blueRange);

    // Adjust the RGB values based on the scales
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8* pixel = pixels + y * rowstride + x * channels;
            guint8* tempPixel = tempPixels + y * rowstride + x * channels;

            tempPixel[0] = (guint8)(pixel[0] * (redValue / maxRed));
            tempPixel[1] = (guint8)(pixel[1] * (greenValue / maxGreen));
            tempPixel[2] = (guint8)(pixel[2] * (blueValue / maxBlue));
        }
    }

    // Update the previewBoxWithImage structure
    g_object_unref(previewBoxWithImage->tempPixbuf);
    previewBoxWithImage->tempPixbuf = tempPixbuf;
    updatePreviewBox(previewBoxWithImage);

    g_message("Image RGB adjusted!");
}


void invertColor(GtkWidget* button, gpointer imageFile) {
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



void adjustTransparency(GtkWidget *scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = (PreviewBoxWithImage *)imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust transparency!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        double transparencyValue = gtk_range_get_value(GTK_RANGE(scale));

        // Ensure transparency value is within valid range [0.0, 1.0]
        transparencyValue = CLAMP(transparencyValue, 0.0, 1.0);

        GdkPixbuf *transparencyPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(transparencyPixbuf);
        int height = gdk_pixbuf_get_height(transparencyPixbuf);
        int channels = gdk_pixbuf_get_n_channels(transparencyPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(transparencyPixbuf);

        guint8 *pixels = gdk_pixbuf_get_pixels(transparencyPixbuf);

        // Iterate over each pixel and adjust the transparency value
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                guint8 *pixel = pixels + y * rowstride + x * channels;

                // Adjust the alpha channel (transparency)
                pixel[3] = (guint8)(pixel[3] * transparencyValue);
            }
        }

        g_object_unref(previewBoxWithImage->originalPixbuf);
        previewBoxWithImage->originalPixbuf = transparencyPixbuf;

        updatePreviewBox(previewBoxWithImage);
        g_message("Transparency adjusted successfully!");
    }
}
