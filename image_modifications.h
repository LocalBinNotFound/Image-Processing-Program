#ifndef IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
#define IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

#include <gtk/gtk.h>

void adjustBrightness(GtkWidget* scale, gpointer imageFile);
void adjustContrast(GtkWidget* scale, gpointer imageFile);
void gaussianBlur(GtkWidget* button, gpointer imageFile);
void laplacianSharpen(GtkWidget* button, gpointer imageFile);
void turnIntoGrayscale(GtkWidget* button, gpointer imageFile);
void adjustGrayscale(GtkWidget* scale, gpointer imageFile);
void rotateByDegree();
void mirrorImageUpDown(GtkWidget* scale, gpointer imageFile);
void mirrorImageLeftRight(GtkWidget* scale, gpointer imageFile);
void adjustRGB(GtkWidget* redScale, GtkWidget* greenScale, GtkWidget* blueScale, gpointer imageFile);
void invertColor(GtkWidget *button, gpointer imageFile);
void adjustTransparency(GtkWidget *scale, gpointer imageFile);

#endif // IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

