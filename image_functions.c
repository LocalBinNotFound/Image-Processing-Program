#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

void adjustBrightness(GtkWidget* scale, gpointer imageFile) {
    gdouble value = gtk_range_get_value(GTK_RANGE(scale));

    gdouble brightnessFactor = (value - 50.0) / 50.0;
}

void gaussianBlur(GtkWidget* scale, gpointer imageFile) {

}

void laplacianSharpen(GtkWidget* scale, gpointer imageFile) {

}

void rotateImage() {

}

void invertColor() {

}

void grayscale(GtkWidget* scale, gpointer imageFile) {

}

void mirrorImage() {

}