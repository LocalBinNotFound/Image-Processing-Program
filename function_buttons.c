/* Code Bibliography:
 * 1. "Reading and writing images with the stb_image libraries" by Paul (https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/)
 *      used to load and write images
 *
 * 2. GTK Tutorial by Georgia Institute of Technology (https://www.cc.gatech.edu/data_files/public/doc/gtk/tutorial/gtk_tut.html#toc10)
 *      used to load/write files, create use of buttons
*/


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <gtk/gtk.h>
#include <stdbool.h>
#include "stb/stb_image.h"
#include "function_buttons.h"

GtkWidget* brightnessScale;
GtkWidget* contrastScale;
GtkWidget* redScale;
GtkWidget* greenScale;
GtkWidget* blueScale;
GtkWidget* transparencyScale;

void setBrightnessScale(GtkWidget* scale) {
    brightnessScale = scale;
}
void setContrastScale(GtkWidget* scale) {
    contrastScale = scale;
}
void setRGBScales(GtkWidget* red, GtkWidget* green, GtkWidget* blue) {
    redScale = red;
    greenScale = green;
    blueScale = blue;
}

void setTransparencyScale(GtkWidget* scale) {
    transparencyScale = scale;
}

// call this function to update the preview image whenever originalPixbuf has been modified
void updatePreviewBox(PreviewBoxWithImage *previewBoxWithImage) {
    GtkWidget *previewBox = previewBoxWithImage->previewBox;
    GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;

    if (originalPixbuf != NULL) {
        int boxWidth, boxHeight;
        gtk_widget_get_size_request(previewBox, &boxWidth, &boxHeight);
        double aspectRatio = (double)gdk_pixbuf_get_width(originalPixbuf) / (double)gdk_pixbuf_get_height(originalPixbuf);
        int newWidth, newHeight;
        if (aspectRatio > 1.0) {
            newWidth = boxWidth;
            newHeight = (int)(boxWidth / aspectRatio);
        } else {
            newWidth = (int)(boxHeight * aspectRatio);
            newHeight = boxHeight;
        }
        GdkPixbuf *scaledPixbuf = gdk_pixbuf_scale_simple(originalPixbuf, newWidth, newHeight, GDK_INTERP_BILINEAR);
        GtkWidget *previewImageWidget = gtk_image_new_from_pixbuf(scaledPixbuf);
        if (previewBoxWithImage->previewImageWidget != NULL) {
            gtk_container_remove(GTK_CONTAINER(previewBox), previewBoxWithImage->previewImageWidget);
        }
        previewBoxWithImage->previewImageWidget = previewImageWidget;
        gtk_container_add(GTK_CONTAINER(previewBox), previewImageWidget);
        gtk_widget_show_all(previewBox);
    }
}

// line 75 to 88 from reference 2
void openButtonClicked(GtkWidget *button, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;
    gint res;

    GtkWidget *fileChooser = gtk_file_chooser_dialog_new("Open File",
                                                         GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                                         "_Cancel",
                                                         GTK_RESPONSE_CANCEL,
                                                         "_Open",
                                                         GTK_RESPONSE_ACCEPT,
                                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(fileChooser));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(fileChooser);
        filename = gtk_file_chooser_get_filename(chooser);

        int width, height, channels;
        unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
        if (image == NULL) {
            printf("Failed to load image!\n");
        } else {
            int rowstride = width * channels;
            unsigned char *alignedImage = (unsigned char *)g_malloc(height * rowstride);
            memcpy(alignedImage, image, height * rowstride);

            GdkPixbuf *originalPixbuf = gdk_pixbuf_new_from_data(alignedImage, GDK_COLORSPACE_RGB, channels == 4,
                                                                 8, width, height, rowstride, NULL, NULL);
            previewBoxWithImage->originalPixbuf = originalPixbuf;
            previewBoxWithImage->referencePixbuf = gdk_pixbuf_copy(originalPixbuf);
            previewBoxWithImage->preservedPixbuf = gdk_pixbuf_copy(originalPixbuf);
            previewBoxWithImage->adjustments.brightness = 0.0;
            previewBoxWithImage->adjustments.contrast = 0.0;
            previewBoxWithImage->adjustments.r = 0.0;
            previewBoxWithImage->adjustments.g = 0.0;
            previewBoxWithImage->adjustments.b = 0.0;

            char msg[100];
            if (channels == 4) sprintf(msg, "This image has %d channels.", channels);
            else sprintf(msg, "This image has %d channels!\nTransparency adjustment not available!", channels);
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "%s", msg);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            stbi_image_free(image);
            updatePreviewBox(previewBoxWithImage);
        }
        g_free(filename);
    }
    gtk_widget_destroy(fileChooser);
}

// line 137 to 151 from reference 2
void saveButtonClicked(GtkWidget *button, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;
    GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save Image",
                                         GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(chooser);

        if (originalPixbuf != NULL) {
            bool success = gdk_pixbuf_save(originalPixbuf, filename, "png", NULL, NULL);
            if (success) {
                g_message("Image saved successfully.");
            } else {
                g_message("Failed to save the image.");
            }
        } else {
            g_message("No image available to save.");
        }
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void clearButtonClicked(GtkWidget *button, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage != NULL) {
        GdkPixbuf *originalPixbuf = previewBoxWithImage->originalPixbuf;

        if (originalPixbuf != NULL) {
            g_object_unref(originalPixbuf);
            previewBoxWithImage->originalPixbuf = NULL;
        }
        if (previewBoxWithImage->previewImageWidget != NULL) {
            gtk_container_remove(GTK_CONTAINER(previewBoxWithImage->previewBox),
                                 previewBoxWithImage->previewImageWidget);
            previewBoxWithImage->previewImageWidget = NULL;
        }
        gtk_range_set_value(GTK_RANGE(brightnessScale), 0.0);
        gtk_range_set_value(GTK_RANGE(contrastScale), 0.0);
        gtk_range_set_value(GTK_RANGE(redScale), 0.0);
        gtk_range_set_value(GTK_RANGE(greenScale), 0.0);
        gtk_range_set_value(GTK_RANGE(blueScale), 0.0);
        gtk_range_set_value(GTK_RANGE(transparencyScale), 0.0);
        updatePreviewBox(previewBoxWithImage);
    }
}


void resetButtonClicked(GtkWidget *button, gpointer imageFile) {
    PreviewBoxWithImage *previewBoxWithImage = imageFile;

    if (previewBoxWithImage == NULL) {
        g_message("Nothing to be reset!");
        return;
    }
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Are you sure you want to reset?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Warning!");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_YES) {
        GdkPixbuf *preservedPixbuf = previewBoxWithImage->preservedPixbuf;
        if (preservedPixbuf != NULL) {
            gtk_range_set_value(GTK_RANGE(brightnessScale), 0.0);
            gtk_range_set_value(GTK_RANGE(contrastScale), 0.0);
            gtk_range_set_value(GTK_RANGE(redScale), 0.0);
            gtk_range_set_value(GTK_RANGE(greenScale), 0.0);
            gtk_range_set_value(GTK_RANGE(blueScale), 0.0);
            gtk_range_set_value(GTK_RANGE(transparencyScale), 0.0);
            g_object_unref(previewBoxWithImage->originalPixbuf);
            previewBoxWithImage->originalPixbuf = gdk_pixbuf_copy(preservedPixbuf);
            g_object_unref(previewBoxWithImage->referencePixbuf);
            previewBoxWithImage->referencePixbuf = gdk_pixbuf_copy(preservedPixbuf);
        }
        updatePreviewBox(previewBoxWithImage);
    }

    gtk_widget_destroy(dialog);
}

