#include <gtk/gtk.h>
#include "buttonActions.h"
#include "image_functions.h"

typedef struct previewBoxWithImage {
    GtkWidget *previewBox;
    GtkWidget *imageWidget;
    GdkPixbuf *pixbuf;
} PreviewBoxWithImage;

GtkWidget* scaleBarBox(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* scale = gtk_scale_new_with_range(orientation, 0.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(scale, 200, -1);
    gtk_range_set_value(GTK_RANGE(scale), 50.0);

    gtk_box_pack_start(GTK_BOX(box), titleLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);

    return box;
}

GtkWidget* tripleScaleBarBox(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* rScale = gtk_scale_new_with_range(orientation, 0.0, 255.0, 1.0);
    GtkWidget* gScale = gtk_scale_new_with_range(orientation, 0.0, 255.0, 1.0);
    GtkWidget* bScale = gtk_scale_new_with_range(orientation, 0.0, 255.0, 1.0);
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
    GtkWidget* framedBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(frame), content);
    gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
    gtk_box_pack_start(GTK_BOX(framedBox), frame, TRUE, TRUE, 10);
    return framedBox;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Initialize main window
    GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Image Wicked!");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);

    // Create boxes
    GtkWidget *overallBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gchar *currentFolder = g_path_get_dirname(__FILE__);
    GtkWidget *image = gtk_image_new_from_file(g_build_filename(currentFolder, "wicked.jpeg", NULL));
    GtkWidget *imageBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *functionsAndPreviewBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *leftFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *rightFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *previewBox = gtk_event_box_new();
    GtkWidget *brightnessBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Brightness");
    GtkWidget *softenBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Blur");
    GtkWidget *sharpenBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Sharpen");
    GtkWidget *contrastBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Contrast");
    GtkWidget *grayscaleBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Grayscale");
    GtkWidget *RGBBox = tripleScaleBarBox(GTK_ORIENTATION_HORIZONTAL, "RGB");
    GtkWidget *rotateBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *mirrorBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *mirrorButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *rotateButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

    // Initialize buttons
    GtkWidget *openButton = gtk_button_new_with_label("Open");
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    GtkWidget *exitButton = gtk_button_new_with_label("Exit");
    GtkWidget *invertColorButton = gtk_button_new_with_label("Invert Color");
    GtkWidget *rotateLeftButton = gtk_button_new_with_label("L");
    GtkWidget *rotateRightButton = gtk_button_new_with_label("R");
    GtkWidget *mirrorUpDownButton = gtk_button_new_with_label("UP/DOWN");
    GtkWidget *mirrorLeftRightButton = gtk_button_new_with_label("LEFT/RIGHT");

    // Set properties for previewBox
    gtk_widget_set_size_request(previewBox, 270, 480);
    GtkWidget *previewBoxWithBorder = boxWithBorder(previewBox);

    // Create labels and entry box
    GtkWidget *rotateLabel = gtk_label_new("Rotation by Degree");
    GtkWidget *rotateAngleTxtBox = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(rotateAngleTxtBox), "Enter rotation degree here");
    GtkWidget *mirrorLabel = gtk_label_new("Mirror Image");

    /*
    GtkWidget *creditsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *creditText =  gtk_label_new("Authors:\n"
                                           "\tJunjie Fang - LocalBinNotFound\n"
                                           "\tWeijian Huang - learningmachine999\n"
                                           "\tQirui Yang - Antonyyqr\n"
                                           "\tXiyuan Tu - XiyuanTu");
    gtk_box_pack_start(GTK_BOX(creditsBox), creditText, FALSE, FALSE, 0);
     */

    // Assign boxes
    gtk_box_set_center_widget(GTK_BOX(imageBox), image);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), leftFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), previewBoxWithBorder, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), rightFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), brightnessBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), contrastBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), softenBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), sharpenBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), grayscaleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), RGBBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), rotateBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), invertColorButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), mirrorBox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(rotateBox), rotateLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateAngleTxtBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateLeftButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateRightButton, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorUpDownButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorLeftRightButton, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(overallBox), imageBox, FALSE, FALSE ,0);
    gtk_box_pack_start(GTK_BOX(overallBox), functionsAndPreviewBox, FALSE, FALSE, 0);
    gtk_box_set_spacing(GTK_BOX(leftFunctionBox), 50);
    gtk_box_set_spacing(GTK_BOX(rightFunctionBox),50);

    // Assign buttons
    GtkWidget *mainButtonsBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(mainButtonsBox), GTK_BUTTONBOX_SPREAD);
    gtk_box_pack_start(GTK_BOX(overallBox), mainButtonsBox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), openButton);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), saveButton);
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), exitButton);

    // RGB
    GtkWidget *rScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(RGBBox))->next->data);
    GtkWidget *gScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(RGBBox))->next->next->data);
    GtkWidget *bScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(RGBBox))->next->next->next->data);
    g_signal_connect(rScale, "value-changed", G_CALLBACK(adjustRGB), previewBox);
    g_signal_connect(gScale, "value-changed", G_CALLBACK(adjustRGB), previewBox);
    g_signal_connect(bScale, "value-changed", G_CALLBACK(adjustRGB), previewBox);

    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)malloc(sizeof(PreviewBoxWithImage));
    previewBoxWithImage->previewBox = previewBox;
    previewBoxWithImage->imageWidget = NULL;
    previewBoxWithImage->pixbuf = NULL;

    // Main buttons
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBoxWithImage);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), previewBoxWithImage);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    // Brightness
    GtkWidget *brightnessScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(brightnessBox))->next->data);
    g_signal_connect(brightnessScale, "value-changed", G_CALLBACK(adjustBrightness), NULL);

    // Add all widgets to the main window
    gtk_container_add(GTK_CONTAINER(mainWindow), overallBox);
    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}