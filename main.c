#include <gtk/gtk.h>
#include "UI.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // main buttons
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

    GtkWidget *leftFunctionalBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vBox), leftFunctionalBox, FALSE, FALSE, 0);

    // need to adjust boxes
    GtkWidget *brightnessBox;
    GtkWidget *brightnessScale;
    brightnessBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(leftFunctionalBox), brightnessBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(brightnessBox),gtk_label_new("Brightness"), FALSE, FALSE, 0);
    brightnessScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(brightnessScale), FALSE);
    gtk_scale_set_has_origin(GTK_SCALE(brightnessScale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(brightnessScale), GTK_POS_RIGHT);
    gtk_box_pack_start(GTK_BOX(brightnessBox), brightnessScale, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vBox), previewBox, TRUE, TRUE, 0);

    GtkWidget *mainButtons = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(mainButtons), GTK_BUTTONBOX_SPREAD);

    gtk_box_pack_start(GTK_BOX(vBox), mainButtons, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainButtons), openButton);
    gtk_container_add(GTK_CONTAINER(mainButtons), saveButton);
    gtk_container_add(GTK_CONTAINER(mainButtons), exitButton);

    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBox);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), NULL);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *sharpenBox;
    sharpenBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(leftFunctionalBox), sharpenBox, FALSE, FALSE, 0);

    GtkWidget *rotateBox;
    rotateBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(leftFunctionalBox), rotateBox, FALSE, FALSE, 0);

    GtkWidget *grayscaleBox;
    grayscaleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(leftFunctionalBox), grayscaleBox, FALSE, FALSE, 0);


    gtk_container_add(GTK_CONTAINER(mainWindow), vBox);

    g_signal_connect(brightnessScale, "value-changed", G_CALLBACK(saveButtonClicked), NULL);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}