#include <gtk/gtk.h>
#include "UI.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // test comment
    GtkWidget *mainWindow;
    GtkWidget *openButton;
    GtkWidget *saveButton;
    GtkWidget *exitButton;
    GtkWidget *previewBox;

    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Image Wicked!");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);

    openButton = gtk_button_new_with_label("Open");
    saveButton = gtk_button_new_with_label("Save");
    exitButton = gtk_button_new_with_label("Exit");
    previewBox = gtk_event_box_new();
    gtk_widget_set_name(previewBox, "preview-box");
    gtk_widget_set_size_request(previewBox, 360, 640);

    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vBox), previewBox, TRUE, TRUE, 0);

    GtkWidget *mainButtons = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(mainButtons), GTK_BUTTONBOX_SPREAD);

    gtk_box_pack_start(GTK_BOX(vBox), mainButtons, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainButtons), openButton);
    gtk_container_add(GTK_CONTAINER(mainButtons), saveButton);
    gtk_container_add(GTK_CONTAINER(mainButtons), exitButton);

    gtk_container_add(GTK_CONTAINER(mainWindow), vBox);

    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBox);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), NULL);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}
