#ifndef SPRITE_H
#define SPRITE_H

#include "Matrix4.h"
#include "Mesh.h"
#include "Texture.h"
#include "YUVTexture.h"
//#include "CameraEngine.h"
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

struct RenderColor
{
    float r;
    float g;
    float b;
    float a;
};

struct RenderState
{
    RenderColor color;
    bool useTexture;
};



// 上层封装，负责“这个东西在哪里、大小多少”，是一个会移动、旋转、缩放的对象，比如：按钮

class Sprite    // 精灵(物体)
{
public:

    Sprite(Mesh* mesh, float width, float height);


    void setPosition(float x, float y); //设置位置
    void setScale(float s); // 设置缩放
    void setAngle(float angle);  // 设置旋转

    void setMoveSpeed(float moveSpeed);                 // 设置移动速度
    void setScaleSpeed(float scaleSpeed);           // 设置缩放速度
    void setRotateSpeed(float rotateSpeed);     // 设置旋转速度

    void setParent(Sprite* parent);             // 设置父物件

    void setOrbit(float radius, float speed);       // 设置轨道运动

    Matrix4 getModelMatrix();

    Mesh* getMesh();

    void setColor(float r, float g, float b, float a);
    void setUseTexture(bool useTexture);
    void setTexture(Texture* texture);

    void setYUVTexture(YUVTexture* texture);
    
    YUVTexture* getYUVTexture();

    Texture* getTexture();

    const RenderState& getRenderState() const;

private:

    float x;
    float y;

    float width;
    float height;

    float scale;
    float angle;

    float pivotX;   // 旋转参考点
    float pivotY;

private:

    float moveSpeed;
    float scaleSpeed;
    float rotateSpeed;

    float dir;  // 为scaleSpeed服务

    Mesh* mesh;
    RenderState renderState;
    Texture* texture;
    
    YUVTexture* yuvTexture;

    Sprite* parent;

    bool orbitEnable;
    float orbitRadius;
    float orbitAngle;
    float orbitSpeed;


public:
    void update();

    void updateCamera(const Frame& frame);


};





#endif

