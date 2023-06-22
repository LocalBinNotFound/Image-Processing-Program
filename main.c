#include <gtk/gtk.h>
#include "UI.h"

GtkWidget* scaleBothSides(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* scale = gtk_scale_new_with_range(orientation, -100.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(scale, 200, -1);
    gtk_range_set_value(GTK_RANGE(scale), 0.0);

    gtk_box_pack_start(GTK_BOX(box), titleLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);

    return box;
}

GtkWidget* scalesRGB(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* rScale = gtk_scale_new_with_range(orientation, 0.0, 100.0, 1.0);
    GtkWidget* gScale = gtk_scale_new_with_range(orientation, 0.0, 100.0, 1.0);
    GtkWidget* bScale = gtk_scale_new_with_range(orientation, 0.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(rScale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(rScale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(rScale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(rScale, 200, -1);
    gtk_scale_set_draw_value(GTK_SCALE(gScale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(gScale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(gScale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(gScale, 200, -1);
    gtk_scale_set_draw_value(GTK_SCALE(bScale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(bScale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(bScale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(bScale, 200, -1);

    gtk_box_pack_start(GTK_BOX(box), titleLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), rScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), gScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), bScale, TRUE, TRUE, 0);

    return box;
}

GtkWidget* boxWithBorder(GtkWidget* content) {
    GtkWidget* framedBox = gtk_frame_new(NULL);
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(framedBox), box);
    gtk_box_pack_start(GTK_BOX(box), content, TRUE, TRUE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(framedBox), 10);
    return framedBox;
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *mainWindow;

    GtkWidget *overallBox;
    // bottom box
    GtkWidget *openButton;
    GtkWidget *saveButton;
    GtkWidget *exitButton;
    GtkWidget *mainButtonsBox;
    // top box
    GtkWidget *functionsAndPreviewBox;
    GtkWidget *leftFunctionBox;
    GtkWidget *brightnessBox;
    GtkWidget *softenBox;
    GtkWidget *sharpenBox;
    GtkWidget *rightFunctionBox;
    GtkWidget *contrastBox;
    GtkWidget *grayscaleBox;
    GtkWidget *RGBBox;
    GtkWidget *previewBox = gtk_event_box_new();

    //initialize main window
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Image Wicked!");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);

    // initialize boxes
    overallBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    functionsAndPreviewBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    leftFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    rightFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *previewBoxWithBorder = boxWithBorder(previewBox);
    gtk_widget_set_size_request(previewBox, 360, 640);

    brightnessBox = boxWithBorder(scaleBothSides(GTK_ORIENTATION_HORIZONTAL, "Brightness"));
    softenBox = boxWithBorder(scaleBothSides(GTK_ORIENTATION_HORIZONTAL, "Blur"));
    sharpenBox = boxWithBorder(scaleBothSides(GTK_ORIENTATION_HORIZONTAL, "Sharpen"));
    contrastBox = boxWithBorder(scaleBothSides(GTK_ORIENTATION_HORIZONTAL, "Contrast"));
    grayscaleBox = boxWithBorder(scaleBothSides(GTK_ORIENTATION_HORIZONTAL, "Grayscale"));
    RGBBox = boxWithBorder(scalesRGB(GTK_ORIENTATION_HORIZONTAL, "RGB"));

    // assign boxes
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), leftFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), previewBoxWithBorder, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), rightFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), brightnessBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), softenBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), sharpenBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), contrastBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), grayscaleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), RGBBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(overallBox), functionsAndPreviewBox, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(leftFunctionBox), TRUE);
    gtk_box_set_homogeneous(GTK_BOX(rightFunctionBox), TRUE);

    // assign buttons
    openButton = gtk_button_new_with_label("Open");
    saveButton = gtk_button_new_with_label("Save");
    exitButton = gtk_button_new_with_label("Exit");
    mainButtonsBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(mainButtonsBox), GTK_BUTTONBOX_SPREAD);

    gtk_box_pack_start(GTK_BOX(overallBox), mainButtonsBox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), openButton);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), saveButton);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), exitButton);

    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBox);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), NULL);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(mainWindow), overallBox);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}