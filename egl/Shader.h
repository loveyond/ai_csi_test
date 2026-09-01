
#ifndef SHADER_H
#define SHADER_H

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
/*
             Shader
               |
     +---------+---------+
     |                   |
ColorShader        TextureShader
     |                   |
 纯色矩形                  图片

*/

// Shader 多种绘制的资源：颜色 Shader、Texture Shader、YUV Shader等等
class Shader
{

public:

    bool load( const char* vertexFile, const char* fragmentFile );

    void use();

    GLuint getProgram()
    {
        return program;
    }

private:

    GLuint program;

    char* loadFile( const char* filename );

    GLuint loadShader( GLenum type, const char* source );

};

#endif



