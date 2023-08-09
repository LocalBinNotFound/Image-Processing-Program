#ifndef IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
#define IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

#include <gtk/gtk.h>

void adjustBrightness(GtkWidget* scale, gpointer imageFile);
void adjustContrast(GtkWidget* scale, gpointer imageFile);
void adjustTransparency(GtkWidget* scale, gpointer imageFile);
void gaussianBlur(GtkWidget* button, gpointer imageFile);
void laplacianSharpen(GtkWidget* button, gpointer imageFile);
void turnIntoGrayscale(GtkWidget* button, gpointer imageFile);
void mirrorImageUpDown(GtkWidget* scale, gpointer imageFile);
void mirrorImageLeftRight(GtkWidget* scale, gpointer imageFile);
void adjustR(GtkWidget* scale, gpointer imageFile);
void adjustG(GtkWidget* scale, gpointer imageFile);
void adjustB(GtkWidget* scale, gpointer imageFile);
void invertColor(GtkWidget *button, gpointer imageFile);
void rotateLeft(GtkWidget* button, gpointer data);
void rotateRight(GtkWidget* button, gpointer data);
void applyVintageFilter(GtkWidget* button, gpointer imageFile);
void oldPictureFilter(GtkWidget* button, gpointer imageFile);

#endif // IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

