
#ifndef YUVTEXTURE_H
#define YUVTEXTURE_H

#include "Texture.h"
#include "Frame.h"



/*
YUVTexture
     │
 ┌───┼───┐
 Y   U   V
 它不是一张 Texture，而是一组描述同一张 YUV 图片的 Texture

*/

class YUVTexture
{
public:
    YUVTexture();

    bool create(int width, int height, const Frame& frame);
    void update(const Frame& frame);

    Texture* getY();
    Texture* getU();
    Texture* getV();

private:
    Texture textureY;
    Texture textureU;
    Texture textureV;

    bool created;
};

#endif



