

#include "image_converter.h"

void ImageConverter::YUV420ToRGB(
    const unsigned char* y,
    const unsigned char* u,
    const unsigned char* v,
    int width,
    int height,
    unsigned char* rgb)
{
    int frameSize = width * height;

    int r, g, b;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            int yIndex = j * width + i;
            int uvIndex = (j / 2) * (width / 2) + (i / 2);

            int Y = y[yIndex];
            int U = u[uvIndex] - 128;
            int V = v[uvIndex] - 128;

            r = Y + 1.402 * V;
            g = Y - 0.344136 * U - 0.714136 * V;
            b = Y + 1.772 * U;

            if (r > 255) r = 255;
            if (r < 0) r = 0;

            if (g > 255) g = 255;
            if (g < 0) g = 0;

            if (b > 255) b = 255;
            if (b < 0) b = 0;

            int rgbIndex = yIndex * 3;

            rgb[rgbIndex + 0] = r;
            rgb[rgbIndex + 1] = g;
            rgb[rgbIndex + 2] = b;
        }
    }
}


