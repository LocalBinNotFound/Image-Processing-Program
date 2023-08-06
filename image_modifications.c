#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "function_buttons.h"
#include <math.h>

const double minPixelValue = 0.0;
const double maxPixelValue = 255.0;

void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust brightness!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf *referencePixbuf = previewBoxWithImage->referencePixbuf;
        GdkPixbuf *brightPixbuf = gdk_pixbuf_copy(originalPixbuf);
        double previousScaleValue = previewBoxWithImage->adjustments.brightness;

        double brightnessValue = gtk_range_get_value(GTK_RANGE(scale));

        int width = gdk_pixbuf_get_width(brightPixbuf);
        int height = gdk_pixbuf_get_height(brightPixbuf);
        int channels = gdk_pixbuf_get_n_channels(brightPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(brightPixbuf);
        guint8 *startPixel = gdk_pixbuf_get_pixels(brightPixbuf);
        guint8 *adjustedStartPixel = gdk_pixbuf_get_pixels(referencePixbuf);

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
                    pixel[c] = (guint8) newValue;
                }
            }
        }
        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(brightPixbuf);
        updatePreviewBox(previewBoxWithImage);
        previewBoxWithImage->adjustments.brightness = brightnessValue;
    }
}

void adjustContrast(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust contrast!");
    } else {
        double contrastValue = gtk_range_get_value(GTK_RANGE(scale));
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf *contrastPixbuf = gdk_pixbuf_copy(originalPixbuf);
        double previousScaleValue = previewBoxWithImage->adjustments.contrast;
        double contrastChange = contrastValue - previousScaleValue;

        int width = gdk_pixbuf_get_width(contrastPixbuf);
        int height = gdk_pixbuf_get_height(contrastPixbuf);
        int channels = gdk_pixbuf_get_n_channels(contrastPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(contrastPixbuf);
        guint8 *pixels = gdk_pixbuf_get_pixels(contrastPixbuf);
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
        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(contrastPixbuf);
        updatePreviewBox(previewBoxWithImage);
        previewBoxWithImage->adjustments.contrast = contrastValue;
        g_object_unref(previewBoxWithImage->referencePixbuf);
        previewBoxWithImage->referencePixbuf = gdk_pixbuf_copy(contrastPixbuf);
    }
}

void adjustTransparency(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust transparency!");
    } else {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf *transPixbuf = gdk_pixbuf_copy(originalPixbuf);

        double transparencyValue = gtk_range_get_value(GTK_RANGE(scale));
        double scaledTransparencyValue = 255 - (transparencyValue * 255 / 100);

        int width = gdk_pixbuf_get_width(transPixbuf);
        int height = gdk_pixbuf_get_height(transPixbuf);
        int channels = gdk_pixbuf_get_n_channels(transPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(transPixbuf);
        guint8 *startPixel = gdk_pixbuf_get_pixels(transPixbuf);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8 *pixel = startPixel + y * rowstride + x * channels;
                int alphaIndex = 3;
                double newValue = scaledTransparencyValue;
                newValue = fmax(fmin(newValue, 255), 0);
                pixel[alphaIndex] = (guint8)newValue;
            }
        }
        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(transPixbuf);
        updatePreviewBox(previewBoxWithImage);
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
        double sigma;
        if(strlen(sigmaStr) == 0) {
            sigma = 1.0;
        } else {
            sigma = g_ascii_strtod(sigmaStr, NULL);
        }

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
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to sharpen!");
    } else {
        GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
        GdkPixbuf* sharpenedPixbuf = gdk_pixbuf_copy(originalPixbuf);
        int width = gdk_pixbuf_get_width(sharpenedPixbuf);
        int height = gdk_pixbuf_get_height(sharpenedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(sharpenedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(sharpenedPixbuf);

        guint8* pixels = gdk_pixbuf_get_pixels(sharpenedPixbuf);

        int kernelScaleDown = 8;
        double kernel[5][5] = {{-1, -1, -1, -1, -1},
                            {-1, 2, 2, 2, -1},
                            {-1, 2, 8, 2, -1},
                            {-1, 2, 2, 2, -1},
                            {-1, -1, -1, -1, -1}};
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                kernel[i][j] /= kernelScaleDown;
            }
        }

        for (int y = 2; y < height - 2; ++y) {
            for (int x = 2; x < width - 2; ++x) {
                for (int c = 0; c < channels; ++c) {
                    double sum = 0.0;
                    for (int i = -2; i <= 2; ++i) {
                        for (int j = -2; j <= 2; ++j) {
                            guint8* pixel = pixels + (y + i) * rowstride + (x + j) * channels + c;
                            double value = *pixel * kernel[i + 2][j + 2];
                            sum += value;
                        }
                    }
                    if (sum < 0) sum = 0;
                    if (sum > 255) sum = 255;
                    guint8* sharpenedPixel = pixels + y * rowstride + x * channels + c;
                    *sharpenedPixel = (guint8)CLAMP(sum, 0, 255);
                }
            }
        }

        g_object_unref(originalPixbuf);
        previewBoxWithImage->originalPixbuf = sharpenedPixbuf;
        updatePreviewBox(previewBoxWithImage);
        g_message("Image sharpened successfully!");
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


// Rotate an image by a specified angle using interpolation techniques such as nearest-neighbor technique
void rotateByDegree(int degree, PreviewBoxWithImage* previewBoxWithImage) {
    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to rotate!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);

    double angle = degree * M_PI / 180.0; // Convert degrees to radians

    int dst_width = ceil(fabs(cos(angle)) * width + fabs(sin(angle)) * height);
    int dst_height = ceil(fabs(sin(angle)) * width + fabs(cos(angle)) * height);

    GdkPixbuf *dst = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, dst_width, dst_height);
    gdk_pixbuf_fill(dst, 0x00000000);

    GdkPixbuf *rotated = gdk_pixbuf_rotate_simple(originalPixbuf, degree);
    gdk_pixbuf_composite(rotated, dst, 0, 0, dst_width, dst_height, 0, 0, 1, 1, GDK_INTERP_NEAREST, 255);

    g_object_unref(originalPixbuf);
    previewBoxWithImage->originalPixbuf = dst;
    updatePreviewBox(previewBoxWithImage);
    g_message("Image rotated by %d degrees successfully!", degree);
}

void rotateLeft(GtkWidget* button, gpointer data) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)data;
//    GtkWidget* entry = gtk_widget_get_parent(gtk_widget_get_parent(button));
//    GList* children = gtk_container_get_children(GTK_CONTAINER(entry));
//    GtkWidget* rotateAngleTxtBox = GTK_WIDGET(g_list_nth_data(children, 0));
//    g_list_free(children);
//    const gchar* text = gtk_entry_get_text(GTK_ENTRY(rotateAngleTxtBox));
//    int degree = atoi(text);
    rotateByDegree(90, previewBoxWithImage); // negative to rotate left
}

void rotateRight(GtkWidget* button, gpointer data) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)data;
//    GtkWidget* entry = gtk_widget_get_parent(gtk_widget_get_parent(button));
//    GList* children = gtk_container_get_children(GTK_CONTAINER(entry));
//    GtkWidget* rotateAngleTxtBox = GTK_WIDGET(g_list_nth_data(children, 0));
//    g_list_free(children);
//    const gchar* text = gtk_entry_get_text(GTK_ENTRY(rotateAngleTxtBox));
//    int degree = atoi(text);
    rotateByDegree(270, previewBoxWithImage); // positive to rotate right
}


void mirrorImageUpDown(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to mirror!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);

    GdkPixbuf* mirroredPixbuf = gdk_pixbuf_copy(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(mirroredPixbuf);

    for (int y = 0; y < height / 2; ++y) {
        int mirroredY = height - y - 1;

        guint8* originalRow = pixels + y * rowstride;
        guint8* mirroredRow = pixels + mirroredY * rowstride;
        for (int x = 0; x < width * channels; ++x) {
            guint8 temp = originalRow[x];
            originalRow[x] = mirroredRow[x];
            mirroredRow[x] = temp;
        }
    }

    g_object_unref(previewBoxWithImage->originalPixbuf);
    previewBoxWithImage->originalPixbuf = mirroredPixbuf;
    updatePreviewBox(previewBoxWithImage);
}

void mirrorImageLeftRight(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to mirror!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    int width = gdk_pixbuf_get_width(originalPixbuf);
    int height = gdk_pixbuf_get_height(originalPixbuf);
    int channels = gdk_pixbuf_get_n_channels(originalPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(originalPixbuf);

    GdkPixbuf* mirroredPixbuf = gdk_pixbuf_copy(originalPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(mirroredPixbuf);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width / 2; ++x) {
            int mirroredX = width - x - 1;

            guint8* originalPixel = pixels + y * rowstride + x * channels;
            guint8* mirroredPixel = pixels + y * rowstride + mirroredX * channels;

            for (int c = 0; c < channels; ++c) {
                guint8 temp = originalPixel[c];
                originalPixel[c] = mirroredPixel[c];
                mirroredPixel[c] = temp;
            }
        }
    }
    g_object_unref(previewBoxWithImage->originalPixbuf);
    previewBoxWithImage->originalPixbuf = mirroredPixbuf;
    updatePreviewBox(previewBoxWithImage);
}

void adjustR(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* referencePixbuf = previewBoxWithImage->referencePixbuf;
    GdkPixbuf* redPixbuf = gdk_pixbuf_copy(originalPixbuf);
    double previousR = previewBoxWithImage->adjustments.r;

    int width = gdk_pixbuf_get_width(redPixbuf);
    int height = gdk_pixbuf_get_height(redPixbuf);
    int channels = gdk_pixbuf_get_n_channels(redPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(redPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(redPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(referencePixbuf);

    double redValue = gtk_range_get_value(GTK_RANGE(scale));

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
    g_object_unref(originalPixbuf);
    previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(redPixbuf);
    updatePreviewBox(previewBoxWithImage);
    previewBoxWithImage->adjustments.r = redValue;
}

void adjustG(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf* referencePixbuf = previewBoxWithImage->referencePixbuf;
    GdkPixbuf* greenPixbuf = gdk_pixbuf_copy(originalPixbuf);
    double previousG = previewBoxWithImage->adjustments.g;

    int width = gdk_pixbuf_get_width(greenPixbuf);
    int height = gdk_pixbuf_get_height(greenPixbuf);
    int channels = gdk_pixbuf_get_n_channels(greenPixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(greenPixbuf);
    guint8* pixels = gdk_pixbuf_get_pixels(greenPixbuf);
    guint8* tempPixels = gdk_pixbuf_get_pixels(referencePixbuf);

    double greenValue = gtk_range_get_value(GTK_RANGE(scale));

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
    g_object_unref(originalPixbuf);
    previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(greenPixbuf);
    updatePreviewBox(previewBoxWithImage);
    previewBoxWithImage->adjustments.g = greenValue;
}

void adjustB(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL || previewBoxWithImage->originalPixbuf == NULL) {
        g_message("No image available to adjust RGB!");
        return;
    }

    GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
    GdkPixbuf *referencePixbuf = previewBoxWithImage->referencePixbuf;
    GdkPixbuf *bluePixbuf = gdk_pixbuf_copy(originalPixbuf);
    double previousB = previewBoxWithImage->adjustments.b;

    int width = gdk_pixbuf_get_width(bluePixbuf);
    int height = gdk_pixbuf_get_height(bluePixbuf);
    int channels = gdk_pixbuf_get_n_channels(bluePixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(bluePixbuf);
    guint8 *pixels = gdk_pixbuf_get_pixels(bluePixbuf);
    guint8 *tempPixels = gdk_pixbuf_get_pixels(referencePixbuf);

    double blueValue = gtk_range_get_value(GTK_RANGE(scale));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guint8 *pixel = pixels + y * rowstride + x * channels;
            guint8 *tempPixel = tempPixels + y * rowstride + x * channels;
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
            pixel[2] = (guint8) newValue;
        }
    }
    g_object_unref(originalPixbuf);
    previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(bluePixbuf);
    updatePreviewBox(previewBoxWithImage);
    previewBoxWithImage->adjustments.b = blueValue;
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
    }
}