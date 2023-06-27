
#ifndef IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
#define IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

#include <gtk/gtk.h>

void adjustBrightness(GtkWidget* scale, gpointer imageFile);
void gaussianBlur(GtkWidget* scale, gpointer imageFile);
void laplacianSharpen(GtkWidget* scale, gpointer imageFile);
void rotateImage();
void invertColor();
void grayscale();
void mirrorImage();
void adjustRGB(GtkWidget *rScale, GtkWidget *gScale, GtkWidget *bScale, GtkWidget *previewBox);


#endif //IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
