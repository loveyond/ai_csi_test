

#pragma once

class ImageConverter{
public:
    static void YUV420ToRGB(
        const unsigned char* y,
        const unsigned char* u,
        const unsigned char* v,
        int width,
        int height,
        unsigned char* rgb);

};

