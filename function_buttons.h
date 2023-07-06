#ifndef IMAGE_PROCESSING_PROGRAM_UI_H
#define IMAGE_PROCESSING_PROGRAM_UI_H

#include <gtk/gtk.h>

typedef struct previewBoxWithImage {
    GtkWidget *previewBox;
    GtkWidget *previewImageWidget;
    GdkPixbuf *originalPixbuf;
} PreviewBoxWithImage;

void updatePreviewBox(PreviewBoxWithImage *previewBoxWithImage);
void openButtonClicked(GtkWidget *button, gpointer imageFile);
void saveButtonClicked(GtkWidget *button, gpointer imageFile);
void clearButtonClicked(GtkWidget *button, gpointer imageFile);

#endif //IMAGE_PROCESSING_PROGRAM_UI_H
