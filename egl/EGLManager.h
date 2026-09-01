
#ifndef EGLMANAGER_H
#define EGLMANAGER_H

#include <EGL/egl.h>
//#include <fb.h>

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
    EGLManager 负责“窗口和显示环境”,是最底层，与硬件打交道，并且绑定硬件和openGL ES
*/

class EGLManager
{

public:

    bool init( int width, int height);


    void swap();


private:

    EGLDisplay display;     // EGL 和底层显示系统的连接句柄

    EGLSurface surface;

    EGLContext context;     // OpenGL ES 的运行环境

    EGLConfig config;

    fbdev_window window;
    
};




#endif


