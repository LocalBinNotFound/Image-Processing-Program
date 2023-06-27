
#ifndef IMAGE_PROCESSING_PROGRAM_UI_H
#define IMAGE_PROCESSING_PROGRAM_UI_H

#include <gtk/gtk.h>

void openButtonClicked(GtkWidget *button, gpointer imageFile);
void saveButtonClicked(GtkWidget *button, gpointer imageFile);
void rotateLButtonClicked(GtkWidget *button, gpointer imageFile);
void rotateRButtonClicked(GtkWidget *button, gpointer imageFile);
void invertButtonClicked(GtkWidget *button, gpointer imageFile);
void mirrorUDButtonClicked(GtkWidget *button, gpointer imageFile);
void mirrorLRButtonClicked(GtkWidget *button, gpointer imageFile);
#endif //IMAGE_PROCESSING_PROGRAM_UI_H
