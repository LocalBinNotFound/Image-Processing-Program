#ifndef IMAGE_PROCESSING_PROGRAM_UI_H
#define IMAGE_PROCESSING_PROGRAM_UI_H

#include <gtk/gtk.h>
#include <stdbool.h>

typedef struct adjustments {
    double brightness;
    double contrast;
    double r;
    double g;
    double b;
} Adjustments;

typedef struct previewBoxWithImage {
    GtkWidget *previewBox;
    GtkWidget *previewImageWidget;
    GdkPixbuf *originalPixbuf;
    GdkPixbuf *referencePixbuf;
    GdkPixbuf *preservedPixbuf;
    Adjustments adjustments;
    GtkWidget* sigmaEntry;
    int softenKernelData;
} PreviewBoxWithImage;


void setBrightnessScale(GtkWidget* scale);
void setContrastScale(GtkWidget* scale);
void setRGBScales(GtkWidget* red, GtkWidget* green, GtkWidget* blue);
void setTransparencyScale(GtkWidget* scale);

void updatePreviewBox(PreviewBoxWithImage *previewBoxWithImage);
void openButtonClicked(GtkWidget *button, gpointer imageFile);
void saveButtonClicked(GtkWidget *button, gpointer imageFile);
void clearButtonClicked(GtkWidget *button, gpointer imageFile);
void resetButtonClicked(GtkWidget *button, gpointer imageFile);

#endif //IMAGE_PROCESSING_PROGRAM_UI_H

