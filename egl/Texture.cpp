#include <stdio.h>
#include <stdlib.h>


#include "Texture.h"


Texture::Texture()
{
    id = 0;
}

// 显示图片
bool Texture::create( int width, int height, unsigned char* data)
{
    // 创建一个 OpenGL Texture 对象，并返回一个 ID
    glGenTextures( 1, &id);

    printf("texture id=%d\n", id);
    // 把  这个id的Texture 绑定到 GL_TEXTURE_2D
    glBindTexture( GL_TEXTURE_2D, id);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 当纹理被缩小的时候，怎么采样？GL_LINEAR 会做线性插值，让缩小后的图片比较平滑

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 纹理被放大的时候，怎么采样? GL_LINEAR 会做线性插值，让放大后的图片比较平滑
    /* 
        把 CPU 里的 data 图片数据上传到当前绑定的 GPU Texture        
    */
    glTexImage2D(
        GL_TEXTURE_2D,                  // 二维纹理
        0,                              // 0 = 基础纹理
        GL_RGB,                         // GPU内部把这张纹理保存成RGB三个颜色分量
        width,
        height,
        0,                              // border
        GL_RGB,                         // CPU传过来的数据格式也是RGB
        GL_UNSIGNED_BYTE,               // CPU 里的每个 R、G、B 数据是一个 unsigned char
        data);

    glBindTexture( GL_TEXTURE_2D, 0);

    
    return true;
}


// Y U V
bool Texture::createGray(int width, int height, const unsigned char* data)
{
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R8,                          // GPU 只保存一个 8-bit 通道
        width,
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}


void Texture::updateGray(int width, int height, const unsigned char* data)
{
    glBindTexture(GL_TEXTURE_2D, id);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        height,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


bool Texture::update(int width, int height, unsigned char* data)
{
    glBindTexture(GL_TEXTURE_2D, id);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        height,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}


void Texture::bind(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture( GL_TEXTURE_2D, id);
}

