#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "function_buttons.h"
#include <MagickWand/MagickWand.h>

void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
}

void adjustContrast(GtkWidget* scale, gpointer imageFile) {

}

void gaussianBlur(GtkWidget* scale, gpointer imageFile) {
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