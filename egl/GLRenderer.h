#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "Matrix4.h"
#include "Mesh.h"
#include "Shader.h"
#include "Sprite.h"
#include "Frame.h"

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



// GLRenderer 负责怎么把它画出来，操作 OpenGL 的
// GLRenderer渲染 + Shader着色
class GLRenderer
{

public:

    bool init();

    void draw( Sprite& sprite);//, Frame* frame = nullptr);

    void clear();

    void begin();

    void resize(int w, int h);

private:

    GLuint program;
    GLuint VBO;
    GLuint textureLoc;
    GLint colorLoc;
    GLint renderModeLoc;        // 0 = 纯颜色    1 = RGB 2 = YUV

    GLint texYLoc;
    GLint texULoc;
    GLint texVLoc;


    GLint mvpLoc;     // 矩阵

//    GLint offsetLoc;
    GLint pos;
    
    char *vertexSource;
    char *fragmentSource;

    Shader shader;

    Matrix4 projection;
    Matrix4 view;

    
};

#endif


