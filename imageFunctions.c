void invertColor(unsigned char *image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        unsigned char *pixel = image + (i * 4);
        pixel[0] = 255 - pixel[0]; // Red
        pixel[1] = 255 - pixel[1]; // Green
        pixel[2] = 255 - pixel[2]; // Blue
    }
}

void increaseBrightness() {

}

void decreaseBrightness() {

}

void gaussianBlur() {

}