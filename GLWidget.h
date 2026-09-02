#ifndef GLWIDGET_H
#define GLWIDGET_H

//#pragma once

#include <QOpenGLWidget>
#include <vector>

#include "GLRenderer.h"
#include "Sprite.h"
#include "YUVTexture.h"
#include "Mesh.h"
#include "Texture.h"
#include "Frame.h"
#include "yolov5.h"

/*
    负责接管原来 egl_test 里面的:
        GLRenderer
        Sprite
        YUVTexture
        Texture
        Mesh
*/
/*
    把 OpenGL 渲染放进 Qt Widget 界面里的一个容器
    
    QOpenGLWidget负责与OpenGL环境的事情,类似于egl_test里的EGLManager：
        创建 OpenGL Context
        创建/管理 Surface
        让 OpenGL 与 Qt Widget 联系起来
        在合适的时候调用你的 OpenGL 绘制函数
*/
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);       // explicit 禁止编译器把一个参数自动转换成 GLWidget

    void updateFrame(const Frame& frame);       // // 准备摄像头数据

    void updateObjects(const std::vector<Object>& objects);     // 准备红框数据.

    void drawBox(const Object& obj, int imageWidth, int imageHeight);


protected:
    /*
        OpenGL 环境准备好以后，Qt调用这个函数
    */
    void initializeGL() override;                 // override 明确这个函数是从父类继承来的虚函数，并且要重写
    /*
        需要重新绘制 GLWidget 的时候，Qt 调用这个函数
        类拟于egl_test里的while(1)内容
    */
    void paintGL() override;


private:
    GLRenderer gl_renderer;

    Mesh* quad;
    Mesh* circle;
    
    Mesh* boxMesh;              // 动态更新的mesh

    Texture texture;
    YUVTexture cameraTexture;

    Sprite* camera1;
    Sprite* photo1;
    Sprite* circle1;

    static const int MAX_BOXES = 20;
    Sprite* boxSprites[MAX_BOXES];
    int boxCount;   // 识别的红框数量.
    
    std::vector<Object> objects;    // 用于画ai识别的红框.

};

#endif

