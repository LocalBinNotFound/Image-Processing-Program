#include <gtk/gtk.h>
#include "function_buttons.h"
#include "image_modifications.h"

GtkWidget* createCircleButtons(gint radius) {
    GtkWidget *button = gtk_button_new();

    gint diameter = radius * 2;
    gint border_width = 2;
    GtkWidget *image = gtk_image_new();

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, diameter, diameter);
    cairo_t *cr = cairo_create(surface);
    cairo_set_line_width(cr, border_width);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_arc(cr, radius, radius, radius - (float) (border_width / 2), 0, 2 * G_PI);
    cairo_stroke(cr);
    cairo_set_source_rgba(cr, 1, 1, 1, 0);
    cairo_arc(cr, radius, radius, radius - border_width, 0, 2 * G_PI);
    cairo_fill(cr);
    cairo_destroy(cr);

    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, diameter, diameter);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    g_object_unref(pixbuf);
    cairo_surface_destroy(surface);
    gtk_button_set_image(GTK_BUTTON(button), image);
    return button;
}

static void softenKernelClicked(GtkWidget* button, gpointer userData) {
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)userData;
    int selectedKernel = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "kernel-size"));
    printf("kernel size: %d\n", selectedKernel);
    previewBoxWithImage->softenKernelData = selectedKernel;
}

GtkWidget* scaleNeg100To100(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* titleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* scale = gtk_scale_new_with_range(orientation, -100.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(scale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(scale, 200, -1);
    gtk_range_set_value(GTK_RANGE(scale), 0.0);
    gtk_box_pack_start(GTK_BOX(titleBox), titleLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(titleBox),TRUE);
    gtk_box_pack_start(GTK_BOX(box), titleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), scale, TRUE, TRUE, 0);
    return box;
}

GtkWidget* tripleScaleBarBox(GtkOrientation orientation, const gchar* title) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* titleLabel = gtk_label_new(title);
    GtkWidget* titleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(titleBox), titleLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(titleBox), TRUE);
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
    gtk_box_pack_start(GTK_BOX(box), titleBox, FALSE, FALSE, 0);
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
    gtk_widget_set_size_request(previewBox, 500, 500);
    GtkWidget *previewBoxWithBorder = boxWithBorder(previewBox);
    PreviewBoxWithImage* previewBoxWithImage = (PreviewBoxWithImage*)malloc(sizeof(PreviewBoxWithImage));
    previewBoxWithImage->previewBox = previewBox;
    previewBoxWithImage->previewImageWidget = NULL;
    previewBoxWithImage->originalPixbuf = NULL;
    previewBoxWithImage->referencePixbuf = NULL;

    // overall layout
    GtkWidget *overallBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *functionsAndPreviewBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *leftFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *rightFunctionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), leftFunctionBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), previewBoxWithBorder, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(functionsAndPreviewBox), rightFunctionBox, FALSE, FALSE, 0);
    gtk_box_set_spacing(GTK_BOX(leftFunctionBox),40);
    gtk_box_set_spacing(GTK_BOX(rightFunctionBox),45);

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
    GtkWidget *resetButton = gtk_button_new_with_label("Reset");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), resetButton);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(resetButtonClicked), previewBoxWithImage);
    GtkWidget *clearButton = gtk_button_new_with_label("Clear");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), clearButton);
    g_signal_connect(clearButton, "clicked", G_CALLBACK(clearButtonClicked), previewBoxWithImage);
    GtkWidget *exitButton = gtk_button_new_with_label("Exit");
    gtk_container_add(GTK_CONTAINER(mainButtonsBox), exitButton);
    g_signal_connect(exitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // rgb
    GtkWidget *RGBBox = tripleScaleBarBox(GTK_ORIENTATION_HORIZONTAL, "RGB");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), RGBBox, FALSE, FALSE, 0);
    GList *children = gtk_container_get_children(GTK_CONTAINER(RGBBox));
    GtkWidget *rBox = GTK_WIDGET(g_list_nth_data(children, 1));
    GtkWidget *gBox = GTK_WIDGET(g_list_nth_data(children, 2));
    GtkWidget *bBox = GTK_WIDGET(g_list_nth_data(children, 3));
    GtkWidget *rScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(rBox)), 1));
    GtkWidget *gScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(gBox)), 1));
    GtkWidget *bScale = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(bBox)), 1));
    setRGBScales(rScale, gScale, bScale);
    g_signal_connect(rScale, "value-changed", G_CALLBACK(adjustR), previewBoxWithImage);
    g_signal_connect(gScale, "value-changed", G_CALLBACK(adjustG), previewBoxWithImage);
    g_signal_connect(bScale, "value-changed", G_CALLBACK(adjustB), previewBoxWithImage);

    //brightness
    GtkWidget *brightnessBox = scaleNeg100To100(GTK_ORIENTATION_HORIZONTAL, "Brightness");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), brightnessBox, FALSE, FALSE, 0);
    GtkWidget *brightnessScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(brightnessBox))->next->data);
    setBrightnessScale(brightnessScale);
    g_signal_connect(brightnessScale, "value-changed", G_CALLBACK(adjustBrightness), previewBoxWithImage);

    // contrast
    GtkWidget *contrastBox = scaleNeg100To100(GTK_ORIENTATION_HORIZONTAL, "Contrast");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), contrastBox, FALSE, FALSE, 0);
    GtkWidget *contrastScale = GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(contrastBox))->next->data);
    setContrastScale(contrastScale);
    g_signal_connect(contrastScale, "value-changed", G_CALLBACK(adjustContrast), previewBoxWithImage);

    // transparency
    GtkWidget* transparencyBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* transparencyTitleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* transparencyLabel = gtk_label_new("Transparency");
    GtkWidget* transparencyScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(transparencyScale), TRUE);
    gtk_scale_set_has_origin(GTK_SCALE(transparencyScale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(transparencyScale), GTK_POS_BOTTOM);
    gtk_widget_set_size_request(transparencyScale, 200, -1);
    gtk_box_pack_start(GTK_BOX(transparencyTitleBox), transparencyLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(transparencyTitleBox),TRUE);
    gtk_box_pack_start(GTK_BOX(transparencyBox), transparencyTitleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(transparencyBox), transparencyScale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), transparencyBox, FALSE, FALSE, 0);
    setTransparencyScale(transparencyScale);
    g_signal_connect(transparencyScale, "value-changed", G_CALLBACK(adjustTransparency), previewBoxWithImage);

    // invert color
    GtkWidget *invertColorButton = gtk_button_new_with_label("Invert Color");
    gtk_box_pack_start(GTK_BOX(leftFunctionBox), invertColorButton, FALSE, FALSE, 0);
    g_signal_connect(invertColorButton, "clicked", G_CALLBACK(invertColor), previewBoxWithImage);

    // monochrome
    GtkWidget* monochromeBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* monochromeButton = gtk_button_new_with_label("Monochrome");
    gtk_box_pack_start(GTK_BOX(monochromeBox), monochromeButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), monochromeBox, FALSE, FALSE, 0);
    g_signal_connect(monochromeButton, "clicked", G_CALLBACK(turnIntoGrayscale), previewBoxWithImage);

    // sharpen
    GtkWidget* sharpenBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), sharpenBox, FALSE, FALSE, 0);
    GtkWidget* sharpenButton = gtk_button_new_with_label("Edge Enhancement");
    g_signal_connect(sharpenButton, "clicked", G_CALLBACK(laplacianSharpen), previewBoxWithImage);
    gtk_box_pack_start(GTK_BOX(sharpenBox), sharpenButton, FALSE, FALSE, 0);

    // soften
    GtkWidget* softenBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* softenLabel = gtk_label_new("Gaussian Blur");
    GtkWidget* softenTitleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(softenTitleBox), softenLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(softenTitleBox), TRUE);
    gtk_box_pack_start(GTK_BOX(softenBox), softenTitleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), softenBox, FALSE, FALSE, 0);
    GtkWidget* sigmaLabel = gtk_label_new("Sigma value:");
    GtkWidget* sigmaEntry = gtk_entry_new();
    previewBoxWithImage->sigmaEntry = sigmaEntry;
    previewBoxWithImage->softenKernelData = 3;
    GtkWidget* sigmaBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(sigmaBox), sigmaLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sigmaBox), sigmaEntry, FALSE, FALSE, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(sigmaEntry), "0.1 to 5.0, default 1.0");
    GtkWidget* softenKernel1 = createCircleButtons(5);
    GtkWidget* softenKernel2 = createCircleButtons(7);
    GtkWidget* softenKernel3 = createCircleButtons(9);
    GtkWidget* softenKernel4 = createCircleButtons(11);
    GtkWidget* softenKernel5 = createCircleButtons(13);
    GtkWidget* softenKernel6 = createCircleButtons(15);
    g_object_set_data(G_OBJECT(softenKernel1), "kernel-size", GINT_TO_POINTER(5));
    g_object_set_data(G_OBJECT(softenKernel2), "kernel-size", GINT_TO_POINTER(9));
    g_object_set_data(G_OBJECT(softenKernel3), "kernel-size", GINT_TO_POINTER(13));
    g_object_set_data(G_OBJECT(softenKernel4), "kernel-size", GINT_TO_POINTER(17));
    g_object_set_data(G_OBJECT(softenKernel5), "kernel-size", GINT_TO_POINTER(21));
    g_object_set_data(G_OBJECT(softenKernel6), "kernel-size", GINT_TO_POINTER(25));
    g_signal_connect(softenKernel1, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    g_signal_connect(softenKernel2, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    g_signal_connect(softenKernel3, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    g_signal_connect(softenKernel4, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    g_signal_connect(softenKernel5, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    g_signal_connect(softenKernel6, "clicked", G_CALLBACK(softenKernelClicked), previewBoxWithImage);
    GtkWidget* softenButtonsContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenButtonsContainer), softenKernel6, FALSE, FALSE, 0);
    GtkWidget* blurButton = gtk_button_new_with_label("Blur Image");
    g_signal_connect(blurButton, "clicked", G_CALLBACK(gaussianBlur), previewBoxWithImage);
    gtk_box_pack_start(GTK_BOX(softenBox), sigmaBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenBox), softenButtonsContainer, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(softenBox), blurButton, FALSE, FALSE, 0);

    // mirror
    GtkWidget *mirrorBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* mirrorTitleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *mirrorButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *mirrorUpDownButton = gtk_button_new_with_label("\u2195");
    GtkWidget *mirrorLeftRightButton = gtk_button_new_with_label("\u2194");
    gtk_widget_set_size_request(mirrorUpDownButton, 50, 50);
    gtk_widget_set_size_request(mirrorLeftRightButton, 50, 50);
    GtkWidget *mirrorLabel = gtk_label_new("Mirror Image");
    gtk_box_pack_start(GTK_BOX(mirrorTitleBox), mirrorLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(mirrorTitleBox), TRUE);
    gtk_box_pack_start(GTK_BOX(rightFunctionBox), mirrorBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorTitleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorBox), mirrorButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorUpDownButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mirrorButtonBox), mirrorLeftRightButton, FALSE, FALSE, 0);

    g_signal_connect(mirrorUpDownButton, "clicked", G_CALLBACK(mirrorImageUpDown), previewBoxWithImage);
    g_signal_connect(mirrorLeftRightButton, "clicked", G_CALLBACK(mirrorImageLeftRight), previewBoxWithImage);
    // need to connect buttons to functions


    // rotate
    GtkWidget *rotateBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *rotateButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *rotateLeftButton = gtk_button_new_with_label("\u21BA");
    GtkWidget *rotateRightButton = gtk_button_new_with_label("\u21BB");
    gtk_widget_set_size_request(rotateLeftButton, 50, 50);
    gtk_widget_set_size_request(rotateRightButton, 50, 50);
    GtkWidget *rotateLabel = gtk_label_new("Rotate Image");
    GtkWidget* rotateTitleBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(rotateTitleBox), rotateLabel, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(rotateTitleBox), TRUE);

    gtk_box_pack_start(GTK_BOX(rightFunctionBox), rotateBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateTitleBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateBox), rotateButtonBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateLeftButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotateButtonBox), rotateRightButton, FALSE, FALSE, 0);
    g_signal_connect(rotateLeftButton, "clicked", G_CALLBACK(rotateLeft), previewBoxWithImage);
    g_signal_connect(rotateRightButton, "clicked", G_CALLBACK(rotateRight), previewBoxWithImage);


    // customize using css
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_file(cssProvider, g_file_new_for_path(g_build_filename(currentFolder, "style.css", NULL)), &error);

    if (error != NULL) {
        g_printerr("Error loading CSS: %s\n", error->message);
        g_clear_error(&error);
    }
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

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

    if (previewBoxWithImage->originalPixbuf != NULL) {
        g_object_unref(previewBoxWithImage->originalPixbuf);
    }
    g_free(previewBoxWithImage);
    g_list_free_full(children, (GDestroyNotify)gtk_widget_destroy);

    return 0;
}