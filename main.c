#include <gtk/gtk.h>
#include "UI.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *mainWindow;
    GtkWidget *openButton;
    GtkWidget *exitButton;
    GtkWidget *previewBox;

    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Image Wicked!");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);

    openButton = gtk_button_new_with_label("Open");
    exitButton = gtk_button_new_with_label("Exit");
    previewBox = gtk_event_box_new();
    gtk_widget_set_name(previewBox, "preview-box");

    const gchar *css = "#preview-box { background-color: #ffffff; }";
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(previewBox);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_widget_set_size_request(previewBox, 360, 640);

    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vBox), previewBox, TRUE, TRUE, 0);

    GtkWidget *hButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hButtonBox), GTK_BUTTONBOX_SPREAD);

    gtk_box_pack_start(GTK_BOX(vBox), hButtonBox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(hButtonBox), openButton);
    gtk_container_add(GTK_CONTAINER(hButtonBox), exitButton);

    gtk_container_add(GTK_CONTAINER(mainWindow), vBox);

    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBox);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}
