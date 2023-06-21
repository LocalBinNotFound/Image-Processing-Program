#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

void openButtonClicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *previewBox = GTK_WIDGET(user_data);

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        int width, height, channels;
        unsigned char *image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
        if (image == NULL) {
            printf("Failed to load image!\n");
        } else {
            double aspectRatio = (double)width / (double)height;

            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(image, GDK_COLORSPACE_RGB, TRUE, 8, width, height, width * 4, NULL, NULL);

            int boxWidth, boxHeight;
            gtk_widget_get_size_request(previewBox, &boxWidth, &boxHeight);

            int newWidth, newHeight;
            if (aspectRatio > 1.0) {
                newWidth = boxWidth;
                newHeight = (int) (boxWidth / aspectRatio);
            } else {
                newWidth =  (int) (boxHeight * aspectRatio);
                newHeight = boxHeight;
            }

            GdkPixbuf *scaledPixbuf = gdk_pixbuf_scale_simple(pixbuf, newWidth, newHeight, GDK_INTERP_BILINEAR);

            GtkWidget *imageWidget = gtk_image_new_from_pixbuf(scaledPixbuf);

            gtk_container_foreach(GTK_CONTAINER(previewBox), (GtkCallback)gtk_widget_destroy, NULL);

            gtk_container_add(GTK_CONTAINER(previewBox), imageWidget);

            gtk_widget_show_all(previewBox);

            g_object_unref(pixbuf);
            stbi_image_free(image);
        }
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}



