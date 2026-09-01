
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLES3/gl3.h>

/*
            应用层
            Sprite
              |
        -----------------
        |               |
      Mesh          Texture
        |               |
        -----------------
              |
      GLRenderer + Shader
              |
              |
          OpenGL ES
              |
              |
         EGLManager
              |
              |
          LCD显示
*/

class Texture
{

public:

    Texture();

    bool create( int width, int height, unsigned char* data );          // 显示图片用
    bool createGray(int width, int height, const unsigned char* data);        // 显示Y U V 用
    
    bool update( int width, int height, unsigned char* data );
    void updateGray(int width, int height, const unsigned char* data);

    void bind(int unit);

private:

    GLuint id;

};


#endif


