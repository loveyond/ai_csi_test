
#include "GLWidget.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static const float rectangleVertices[] =
{
    //                // UV    
   0,   0, 0,       0,0,
   1,   0, 0,       1,0,
     0, 1, 0,       0,1,
    
   1,   0, 0,       1,0,
   1, 1, 0,       1,1,
     0, 1, 0,       0,1
};


static const float boxVertices[20] =
{
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 0, 0, 0,
    0, 1, 0, 0, 0
};




/*
    在一个 1 × 1 的正方形里，生成一个圆的“圆心 + 圆周顶点”，每个顶点包含 x、y、z、u、v 5 个数据
    ┌────┬────┬────┬────┬────┐
    │ x  │ y  │ z  │ u  │ v  │
    └────┴────┴────┴────┴────┘
*/
std::vector<float> createCircle(int segments)
{
    std::vector<float> vertices;

    const float centerX = 0.5f;
    const float centerY = 0.5f;
    const float radius = 0.5f;

    // 圆心
    vertices.push_back(centerX);
    vertices.push_back(centerY);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);       // 圆心的uv也是纹理的中心
    vertices.push_back(0.5f);       // 圆心的uv也是纹理的中心

    // 圆周
    for(int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;

        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);

        float u = (x - centerX) / (2.0f * radius) + 0.5f;
        float v = (y - centerY) / (2.0f * radius) + 0.5f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(u);
        vertices.push_back(v);
    }

    return vertices;
}


struct ImageFrame
{
    unsigned char* img;
    int w;
    int h;
    int ch;
};


int jpg2rgb(ImageFrame &image)
{
    image.img = stbi_load(
        "f_20260820_173757.jpg",
        &image.w,
        &image.h,
        &image.ch,
        3
    );
    
    if(!image.img)
    {
        printf("stbi_load failed: %s\n", stbi_failure_reason());
        return -1;
    }
    
    printf("image: %d x %d channels=%d\n", image.w, image.h, image.ch);
    return 0;
}



GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , quad(nullptr)
    , circle(nullptr)
    , camera1(nullptr)
    , photo1(nullptr)
    , circle1(nullptr)
{
    printf("GLWidget constructor\n");
}



void GLWidget::initializeGL()
{
    printf("GL:%s\n", glGetString(GL_VERSION));

// Renderer / Shader
    gl_renderer.init();
    
// Mesh / Texture
    // 创建GPU资源(VAO、VBO)
    quad = new Mesh(rectangleVertices, 6);

    std::vector<float> circleVertices = createCircle(32);
    circle = new Mesh(
        circleVertices.data(),
        circleVertices.size() / 5,
        GL_TRIANGLE_FAN);

    boxMesh = new Mesh(boxVertices, 4, GL_LINE_LOOP);   // GL_LINE_LOOP会自动连接每根线，形成四边形

    // 把 CPU 里的 RGB 图片数据，创建成 GPU 里的 2D Texture
    ImageFrame imageFrame;
    jpg2rgb(imageFrame);   
    texture.create(
        imageFrame.w,
        imageFrame.h,
        imageFrame.img);    
    stbi_image_free(imageFrame.img);

// 创建sprite
    printf("GLWidget width=%d,height=%d\n",width(),height());
    camera1 = new Sprite(quad, width(), height());
    camera1->setPosition(width()/2,height()/2);
    camera1->setMoveSpeed(0);
    camera1->setScaleSpeed(0);
    camera1->setRotateSpeed(0);
    camera1->setYUVTexture(&cameraTexture);

    photo1 = new Sprite(quad, 200, 200);
    photo1->setPosition(100,100);
    photo1->setMoveSpeed(0);
    photo1->setScaleSpeed(0);
    photo1->setRotateSpeed(2);
    photo1->setTexture(&texture);

    circle1 = new Sprite(circle, 0.1, 0.1);  // 设置父子关系后的坐标已经不是坐标了,目前只用于测试,现阶段懒得改了. 0.1*0.1==(0.1*200)*(0.1*200)像素
    circle1->setParent(camera1);         // 设置父子关系
    circle1->setPosition(0.7,0);         // 这里的位置坐标是相对于photo1的坐标: 0.7*0==(0.7*200)*0像素
    circle1->setScaleSpeed(0);
    circle1->setRotateSpeed(0);
    circle1->setColor(1, 1, 0, 1);
    circle1->setMoveSpeed(0);
    circle1->setOrbit(0.7f, 0.02f);

    for(int i = 0; i < MAX_BOXES; i++)
    {
        boxSprites[i] = new Sprite(boxMesh, 0, 0);
    
        boxSprites[i]->setColor(1, 0, 0, 1);    // R = 1 G = 0 B = 0 A = 1
        boxSprites[i]->setMoveSpeed(0);
        boxSprites[i]->setScaleSpeed(0);
        boxSprites[i]->setRotateSpeed(0);
    }
//    boxSprites[0] = new Sprite(boxMesh, 200, 200);
//    boxSprites[0]->setPosition(300, 200);
//    boxSprites[0]->setColor(1, 0, 0, 1);

    printf("initializeGL done\n");
}


// 使用数据进行绘制
void GLWidget::paintGL()
{
    gl_renderer.clear();
    gl_renderer.begin();

    photo1->update();
    circle1->update();
    camera1->update();

//    gl_renderer.draw(*photo1);
//    gl_renderer.draw(*circle1);
    gl_renderer.draw(*camera1);

    for(int i = 0; i < boxCount; i++)
        gl_renderer.draw(*boxSprites[i]);
        
}



// 准备摄像头数据
void GLWidget::updateFrame(const Frame& frame)
{
    camera1->updateCamera(frame);
    update();       // 通知 Qt：重新绘制,即调用paintGL
}



void GLWidget::updateObjects(const std::vector<Object>& newObjects)
{
    boxCount = std::min((int)newObjects.size(), MAX_BOXES);

    float sx = (float)width() / 320.0f;     // 这里的320 和 Yolov5::detect的 ncnn::Mat in = ncnn::Mat::from_pixels_resize(rgb_data, ncnn::Mat::PIXEL_RGB, w, h, 320, 320); 对应
    float sy = (float)height() / 320.0f;

    for(int i = 0; i < boxCount; i++)
    {
        const Object& obj = newObjects[i];

        boxSprites[i]->setPosition(
            (obj.rect.x + obj.rect.width / 2.0f) * sx,
            (obj.rect.y + obj.rect.height / 2.0f) * sy
        );


        boxSprites[i]->setSize(
            obj.rect.width * sx,
            obj.rect.height * sy
        );

    }
}



void GLWidget::resizeGL(int w, int h)
{
    gl_renderer.resize(w, h);
}



//void GLWidget::drawBox(const Object& obj, int imageWidth, int imageHeight)
//{
//    float sx = 1024.0f / imageWidth;
//    float sy = 600.0f / imageHeight;
//
//    float x1 = obj.rect.x * sx;
//    float y1 = obj.rect.y * sy;
//    float x2 = (obj.rect.x + obj.rect.width) * sx;
//    float y2 = (obj.rect.y + obj.rect.height) * sy;
//
//    float vertices[20] =
//    {
//        x1, y1, 0, 0, 0,
//        x2, y1, 0, 0, 0,
//        x2, y2, 0, 0, 0,
//        x1, y2, 0, 0, 0
//    };
//
//    boxMesh->update(vertices, 4);
//
////    gl_renderer.draw(*boxMesh);
//}




