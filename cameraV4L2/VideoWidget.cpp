// 这是gpu画图需要的

#include "VideoWidget.h"

void VideoWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glGenTextures(1, &texY);
    glGenTextures(1, &texU);
    glGenTextures(1, &texV);

    // Y
    glBindTexture(GL_TEXTURE_2D, texY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // U
    glBindTexture(GL_TEXTURE_2D, texU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // V
    glBindTexture(GL_TEXTURE_2D, texV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void VideoWidget::setFrame(uint8_t *y,
                           uint8_t *u,
                           uint8_t *v,
                           int w, int h)
{
    width = w;
    height = h;

    glBindTexture(GL_TEXTURE_2D, texY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
                 w, h, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, y);

    glBindTexture(GL_TEXTURE_2D, texU);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
                 w/2, h/2, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, u);

    glBindTexture(GL_TEXTURE_2D, texV);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
                 w/2, h/2, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, v);

    update(); // 触发重绘
}

void VideoWidget::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texY);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texU);

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texV);

   // shader draw
   drawRectangle();
}



