
#include <cmath>


#include "Sprite.h"



// 精灵(物体)

Sprite::Sprite(Mesh* mesh, float width, float height)
{
    this->mesh = mesh;
    this->width = width;
    this->height = height;
    this->texture = nullptr;
    this->yuvTexture = nullptr;
    parent = nullptr;

    orbitEnable = false;
    orbitRadius = 0.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.0f;

    renderState.color = {1, 1, 1, 1};
    renderState.useTexture = false;


    x = 0;
    y = 0;

    scale = 1.0f;
    angle = 0;
    
    dir = 1.0f;
    moveSpeed = 5.0f;
    scaleSpeed = 0.01f;
    rotateSpeed = 1.0f;


    pivotX = 0.5f;//width / 2.0f;      // 在中心
    pivotY = 0.5f;//height / 2.0f;
}


// 轨道运动
void Sprite::setOrbit(float radius, float speed)
{
    orbitEnable = true;
    orbitRadius = radius;
    orbitSpeed = speed;
}



void Sprite::setPosition(float x,float y)
{
    this->x = x;
    this->y = y;
}


void Sprite::setScale(float s)
{
    scale = s;
}


void Sprite::setAngle(float angle)
{
    this->angle = angle;
}

void Sprite::setMoveSpeed(float moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

void Sprite::setScaleSpeed(float scaleSpeed)
{
    this->scaleSpeed = scaleSpeed;
}

void Sprite::setRotateSpeed(float rotateSpeed)
{
    this->rotateSpeed = rotateSpeed;
}

void Sprite::setColor(float r, float g, float b, float a)
{
    renderState.color = {r, g, b, a};
}

void Sprite::setUseTexture(bool useTexture)
{
    renderState.useTexture = useTexture;
}

void Sprite::setTexture(Texture* texture)
{
    this->texture = texture;
    renderState.useTexture = true;
}


void Sprite::setParent(Sprite* parent)
{
    this->parent = parent;
}



void Sprite::setYUVTexture(YUVTexture* texture)
{
    yuvTexture = texture;
}



YUVTexture* Sprite::getYUVTexture()
{
    return yuvTexture;
}



Matrix4 Sprite::getModelMatrix()
{
    Matrix4 local =
        Matrix4::translate(x,y,0) *
        Matrix4::translate(pivotX,pivotY,0) *   // // 然后恢复原来的中心
        Matrix4::rotate(angle) *
        Matrix4::scale(width * scale, height * scale,1) *
        Matrix4::translate(-pivotX,-pivotY,0);  // 先把旋转中心移动到原点

    if(parent)
        return parent->getModelMatrix() * local;    // 世界变换 = 父节点世界变换 × 自己的局部变换
    return local;    

}

const RenderState& Sprite::getRenderState() const
{
    return renderState;
}

Texture* Sprite::getTexture()
{
    return texture;
}

Mesh* Sprite::getMesh()
{
    return mesh;
}

void Sprite::update()
{
    // 移动
    x += moveSpeed;
    if(x >= 1024)
        x = -width;

    // 缩放
    scale += scaleSpeed  * dir;
    if(scale >= 1.5f)
    {
        scale = 1.5f;
        dir = -1.0f;
    }
    else if(scale <= 0.1f)
    {
        scale = 0.1f;
        dir = 1.0f;
    }

    // 自旋转
    angle += rotateSpeed;
    if(angle >= 360)
        angle = 0;

    // 轨道运动
    if(orbitEnable)
    {
        orbitAngle += orbitSpeed;

        x = cosf(orbitAngle) * orbitRadius;
        y = sinf(orbitAngle) * orbitRadius;
    }


}



void Sprite::updateCamera(const Frame& frame)
{
//    Frame frame;

    if(yuvTexture)
        yuvTexture->update(frame);
}



