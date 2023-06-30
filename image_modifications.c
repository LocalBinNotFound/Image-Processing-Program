#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "function_buttons.h"
#include <math.h>


void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
}

void adjustContrast(GtkWidget* scale, gpointer imageFile) {

}

void gaussianBlur(GtkWidget* scale, gpointer imageFile) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)imageFile;
    GdkPixbuf* originalPixbuf = previewBoxWithImage->originalPixbuf;

    if (originalPixbuf != NULL) {
        GdkPixbuf* blurredPixbuf = gdk_pixbuf_copy(originalPixbuf);
        int width = gdk_pixbuf_get_width(blurredPixbuf);
        int height = gdk_pixbuf_get_height(blurredPixbuf);
        int channels = gdk_pixbuf_get_n_channels(blurredPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(blurredPixbuf);

        guint8* pixels = gdk_pixbuf_get_pixels(blurredPixbuf);

        double sigma = 5.0;  // Adjust the sigma value for desired blurring effect
        int kernelSize = 25;  // Adjust the kernel size for desired blurring effect

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
    } else {
        g_message("No image available to blur!");
    }
}

void laplacianSharpen(GtkWidget* scale, gpointer imageFile) {
}

void adjustGrayscale(GtkWidget* scale, gpointer imageFile) {
}

void rotateByDegree() {
}

void mirrorImage() {
}

void adjustRGB(GtkWidget *rScale, GtkWidget *gScale, GtkWidget *bScale, GtkWidget *previewBox) {
}

void invertColor(GtkWidget *button, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;
    GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;

    if (originalPixbuf != NULL) {
        GdkPixbuf *invertedPixbuf = gdk_pixbuf_copy(originalPixbuf);

        int width = gdk_pixbuf_get_width(invertedPixbuf);
        int height = gdk_pixbuf_get_height(invertedPixbuf);
        int channels = gdk_pixbuf_get_n_channels(invertedPixbuf);
        int rowstride = gdk_pixbuf_get_rowstride(invertedPixbuf);

        guint8 *pixels = gdk_pixbuf_get_pixels(invertedPixbuf);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                guint8 *pixel = pixels + y * rowstride + x * channels;
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
    } else {
        g_message("No image available to invert!");
    }
}