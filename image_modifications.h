
#ifndef IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
#define IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H

#include <gtk/gtk.h>
//#include <MagickWand/MagickWand.h>

void adjustBrightness(GtkWidget* scale, gpointer imageFile);
void adjustContrast(GtkWidget* scale, gpointer imageFile);
void gaussianBlur(GtkWidget* scale, gpointer imageFile);
void laplacianSharpen(GtkWidget* scale, gpointer imageFile);
void adjustGrayscale(GtkWidget* scale, gpointer imageFile);
void rotateByDegree();
void mirrorImage();
void adjustRGB(GtkWidget *rScale, GtkWidget *gScale, GtkWidget *bScale, GtkWidget *previewBox);
void invertColor(GtkWidget *button, gpointer imageFile);


#endif //IMAGE_PROCESSING_PROGRAM_IMAGE_FUNCTIONS_H
