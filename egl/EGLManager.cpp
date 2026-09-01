#include <stdio.h>
#include <string.h>

#include "EGLManager.h"

/*
    属性 = 值
    属性 = 值
    ...
    结束
*/
static EGLint attribs[] =
{
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,           // 创建 窗口Surface类型

    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,    // 用OpenGL ES 3.0，实际版本号是：OpenGL ES 3.2 v1.r20p0-01rel0.8802b4327f9e69cec3438957f40dba06
                                                // 用ES 2.0也行
    EGL_RED_SIZE, 8,                            // RGB888
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,

    EGL_NONE
};

static EGLint contextAttribs[] =
{
    EGL_CONTEXT_CLIENT_VERSION,         // OpenGL ES 3.0。用2.0也行
    3,
    EGL_NONE
};


// EGL负责"在哪里画":连接显示设备  及参数
// 流程：eglGetDisplay -> eglInitialize -> eglChooseConfig -> eglCreateWindowSurface -> eglCreateContext -> eglMakeCurrent
bool EGLManager::init( int width, int height)
{
    printf("EGL test start\n");

// 第一步 获取显示设备
    
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);       // 使用系统默认显示设备    ,如/dev/fb0
    if(display == EGL_NO_DISPLAY)
    {
        printf("eglGetDisplay failed\n");
        return -1;
    }
    printf("eglGetDisplay success\n");

    EGLint major;
    EGLint minor;

// 第二步 初始化 EGL
    if(!eglInitialize(display, &major, &minor))
    {
        printf("eglInitialize failed\n");
        return -1;
    }
    printf("EGL version %d.%d\n", major, minor);    // 版本 EGL 1.4

// 第三步 选择 EGL 配置
    EGLint num_config;
    
    if(!eglChooseConfig(
            display,
            attribs,
            &config,
            1,                  // 最多返回几个Config，这里只需要1个
            &num_config))
    {
        printf("eglChooseConfig failed\n");
        return -1;
    }
    printf("config count=%d\n", num_config);

// 第四步 创建显示窗口：[ EGL_DEFAULT_DISPLAY(/dev/fb0)和 EGL 的连接 ]

    window.width = width;
    window.height = height;
    // 创建   EGL Surface        
    surface = eglCreateWindowSurface(
            display,
            config,
            &window,
            NULL
    );        
    if(surface == EGL_NO_SURFACE)
    {
        printf("eglCreateWindowSurface failed\n");
        return -1;
    }
    printf("surface create success\n");

// 第五步：创建 GLES2 Context        
    
    context = eglCreateContext(
            display,
            config,
            EGL_NO_CONTEXT,
            contextAttribs
    );    
    
    if(context == EGL_NO_CONTEXT)
    {
        printf("eglCreateContext failed\n");
        return -1;
    }
    printf("context create success\n");

// 第六步：绑定 硬件和openGL ES
    if(!eglMakeCurrent(
            display,
            surface,
            surface,
            context))
    {
        printf("eglMakeCurrent failed\n");
        return false;
    }    
    printf("eglMakeCurrent success\n");


// 控制swap间隔：每次 eglSwapBuffers()，至少等待一次显示刷新周期（比如30fps）
    eglSwapInterval(display, 1);

    return true;
}

/*
    swap ret=1 error=0x3000
    swap ret=0 error=0x3003
    swap ret=0 error=0x3003
*/
void EGLManager::swap()
{
    EGLBoolean ret = eglSwapBuffers(display, surface);

//    printf("swap ret=%d error=0x%x\n",
//           ret,
//           eglGetError());
}


