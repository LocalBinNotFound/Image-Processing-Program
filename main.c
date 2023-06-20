#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "image_functions.h"

// tst
void openButtonClicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(user_data),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         "_Select",
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
            g_print("Failed to load image\n");
        } else {
            invertColor(image, width, height);

            char outputDirectory[256];
            strncpy(outputDirectory, filename, sizeof(outputDirectory));
            char *lastSlash = strrchr(outputDirectory, '/');
            if (lastSlash != NULL) {
                *(lastSlash + 1) = '\0';
            }

            char outputImagePath[256];
            snprintf(outputImagePath, sizeof(outputImagePath), "%soutput.png", outputDirectory);

            int result = stbi_write_png(outputImagePath, width, height, channels, image, width * channels);
            if (result == 0) {
                g_print("Error writing image\n");
            } else {
                g_print("Image processing completed successfully! Modified image saved as: %s\n", outputImagePath);
            }

            stbi_image_free(image);
        }
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void exitButtonClicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    gtk_widget_destroy(window);
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *openButton;
    GtkWidget *exitButton;
    GtkWidget *button_box;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), button_box);

    openButton = gtk_button_new_with_label("Open");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), window);
    gtk_container_add(GTK_CONTAINER(button_box), openButton);

    exitButton = gtk_button_new_with_label("Exit");
    g_signal_connect(exitButton,"clicked", G_CALLBACK(exitButtonClicked), window);
    gtk_container_add(GTK_CONTAINER(button_box), exitButton);

    gtk_widget_show_all(window);

    gtk_main();
    // test 1
    return 0;
}
