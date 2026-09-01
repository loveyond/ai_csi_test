
#include "YUVTexture.h"

YUVTexture::YUVTexture()
    : created(false)
{
}


// 给GPU 分配三个 Texture
bool YUVTexture::create(int width, int height, const Frame& frame)
{
    if(!textureY.createGray(width, height, frame.y.data()))
        return false;

    if(!textureU.createGray(width / 2, height / 2, frame.u.data()))
        return false;

    if(!textureV.createGray(width / 2, height / 2, frame.v.data()))
        return false;

    created = true;

    return true;
}


// 以后每一帧摄像头进来，只更新数据
void YUVTexture::update(const Frame& frame)
{
    if(!created)
    {
        create(frame.width, frame.height, frame);
        return;
    }

    textureY.updateGray(
        frame.width,
        frame.height,
        frame.y.data());

    textureU.updateGray(
        frame.width / 2,
        frame.height / 2,
        frame.u.data());

    textureV.updateGray(
        frame.width / 2,
        frame.height / 2,
        frame.v.data());
}



Texture* YUVTexture::getY()
{
    return &textureY;
}


Texture* YUVTexture::getU()
{
    return &textureU;
}


Texture* YUVTexture::getV()
{
    return &textureV;
}






