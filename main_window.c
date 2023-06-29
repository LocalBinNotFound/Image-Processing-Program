#include <gtk/gtk.h>
#include "function_buttons.h"
#include "image_modifications.h"

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
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* rBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* gBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* bBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* rLabel = gtk_label_new("R");
    GtkWidget* gLabel = gtk_label_new("G");
    GtkWidget* bLabel = gtk_label_new("B");
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

    gtk_box_pack_start(GTK_BOX(rBox), rLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rBox), rScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gBox), gLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(gBox), gScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(bBox), bLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bBox), bScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), titleLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), rBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), gBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), bBox, FALSE, FALSE, 0);
    return box;
}

GtkWidget* boxWithBorder(GtkWidget* content) {
    GtkWidget* framedBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(frame), content);
    gtk_container_set_border_width(GTK_CONTAINER(frame), 0);
    gtk_box_pack_start(GTK_BOX(framedBox), frame, TRUE, TRUE, 0);
    return framedBox;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // main window
    GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Image Wicked!");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 10);

    // preview box
    GtkWidget *previewBox = gtk_event_box_new();
    gtk_widget_set_size_request(previewBox, 270, 480);
    GtkWidget *previewBoxWithBorder = boxWithBorder(previewBox);
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)malloc(sizeof(PreviewBoxWithImage));
    previewBoxWithImage->previewBox = previewBox;
    previewBoxWithImage->previewImageWidget = NULL;
    previewBoxWithImage->originalPixbuf = NULL;

    // overall layout
    GtkWidget *overallBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *functionsAndPreviewBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *leftFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *rightFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), leftFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), previewBoxWithBorder, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), rightFunctionBox, FALSE, FALSE, 0);
    gtk_box_set_spacing(GTK_BOX(leftFunctionBox), 40);
    gtk_box_set_spacing(GTK_BOX(rightFunctionBox),50);

    // logo
    gchar *currentFolder = g_path_get_dirname(__FILE__);
    GtkWidget *logo = gtk_image_new_from_file(g_build_filename(currentFolder, "wicked.png", NULL));
    GtkWidget *logoBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_center_widget(GTK_BOX(logoBox), logo);
    gtk_box_pack_start(GTK_BOX(overallBox), logoBox, FALSE, FALSE , 0);
    gtk_box_pack_start(GTK_BOX(overallBox), functionsAndPreviewBox, FALSE, FALSE, 0);

    // function buttons
    GtkWidget *mainButtonsBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(mainButtonsBox), GTK_BUTTONBOX_SPREAD);
    gtk_box_pack_start(GTK_BOX(overallBox), mainButtonsBox, FALSE, FALSE, 0);
    GtkWidget *openButton = gtk_button_new_with_label("Open");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), openButton);
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked), previewBoxWithImage);
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), saveButton);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), previewBoxWithImage);
    GtkWidget *exitButton = gtk_button_new_with_label("Exit");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), exitButton);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //brightness
    GtkWidget *brightnessBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Brightness");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), brightnessBox, FALSE, FALSE, 0);
    GtkWidget *brightnessScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(brightnessBox))->next->data);
    g_signal_connect(brightnessScale, "value-changed", G_CALLBACK(adjustBrightness), NULL);

    // contrast
    GtkWidget *contrastBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Contrast");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), contrastBox, FALSE, FALSE, 0);
    GtkWidget *contrastScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(contrastBox))->next->data);
    g_signal_connect(contrastScale, "value-changed", G_CALLBACK(adjustContrast), NULL);

    // soften
    GtkWidget *softenBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Blur");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), softenBox, FALSE, FALSE, 0);
    GtkWidget *softenScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(softenBox))->next->data);
    g_signal_connect(softenScale, "value-changed", G_CALLBACK(gaussianBlur), NULL);

    // sharpen
    GtkWidget *sharpenBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Sharpen");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), sharpenBox, FALSE, FALSE, 0);
    GtkWidget *sharpenScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(sharpenBox))->next->data);
    g_signal_connect(sharpenScale, "value-changed", G_CALLBACK(laplacianSharpen), NULL);

    // grayscale
    GtkWidget *grayscaleBox = scaleBarBox(GTK_ORIENTATION_HORIZONTAL, "Grayscale");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), grayscaleBox, FALSE, FALSE, 0);
    GtkWidget *grayscaleScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(grayscaleBox))->next->data);
    g_signal_connect(grayscaleScale, "value-changed", G_CALLBACK(adjustGrayscale), NULL);

    // rgb
    GtkWidget *RGBBox = tripleScaleBarBox(GTK_ORIENTATION_HORIZONTAL, "RGB");
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), RGBBox, FALSE, FALSE, 0);
    GList *children = gtk_container_get_children(GTK_CONTAINER(RGBBox));
    GtkWidget *rBox = GTK_WIDGET(g_list_nth_data(children, 1));
    GtkWidget *gBox = GTK_WIDGET(g_list_nth_data(children, 2));
    GtkWidget *bBox = GTK_WIDGET(g_list_nth_data(children, 3));
    GtkWidget *rScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(rBox)), 1));
    GtkWidget *gScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(gBox)), 1));
    GtkWidget *bScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(bBox)), 1));
    g_signal_connect(rScale, "value-changed", G_CALLBACK(adjustRGB), NULL);
    g_signal_connect(gScale, "value-changed", G_CALLBACK(adjustRGB), NULL);
    g_signal_connect(bScale, "value-changed", G_CALLBACK(adjustRGB), NULL);

    // mirror
    GtkWidget *mirrorBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *mirrorButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *mirrorUpDownButton = gtk_button_new_with_label("UP/DOWN");
    GtkWidget *mirrorLeftRightButton = gtk_button_new_with_label("LEFT/RIGHT");
    GtkWidget *mirrorLabel = gtk_label_new("Mirror Image");
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), mirrorBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorUpDownButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorLeftRightButton, FALSE, FALSE, 0);
    gtk_box_set_spacing(GTK_BOX(mirrorButtonBox), 10);
    // need to connect buttons to functions


    // rotate
    GtkWidget *rotateBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *rotateButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *rotateLeftButton = gtk_button_new_with_label("L");
    GtkWidget *rotateRightButton = gtk_button_new_with_label("R");
    GtkWidget *rotateLabel = gtk_label_new("Rotation by Degree");
    GtkWidget *rotateAngleTxtBox = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(rotateAngleTxtBox), "Enter rotation degree here");
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), rotateBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateAngleTxtBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateLeftButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateRightButton, FALSE, FALSE, 0);
    // need to connect buttons to functions


    // invert color
    GtkWidget *invertColorButton = gtk_button_new_with_label("Invert Color");
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), invertColorButton, FALSE, FALSE, 0);
    g_signal_connect(invertColorButton, "clicked", G_CALLBACK(invertColor), previewBoxWithImage);

    /*
    GtkWidget *creditsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *creditText =  gtk_label_new("Authors:\n"
                                           "\tJunjie Fang - LocalBinNotFound\n"
                                           "\tWeijian Huang - learningmachine999\n"
                                           "\tQirui Yang - Antonyyqr\n"
                                           "\tXiyuan Tu - XiyuanTu");
    gtk_box_pack_start(GTK_BOX(creditsBox), creditText, FALSE, FALSE, 0);
     */

    // Add all widgets to the main window
    gtk_container_add(GTK_CONTAINER(mainWindow), overallBox);
    gtk_widget_show_all(mainWindow);

    gtk_main();

    g_object_unref(previewBoxWithImage->originalPixbuf);
    g_free(previewBoxWithImage);
    g_list_free_full(children, (GDestroyNotify)gtk_widget_destroy);

    return 0;
}