#pragma once
// 这是gpu画图需要的


class VideoWidget : public QOpenGLWidget,
                    protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

    void setFrame(uint8_t *y,
                  uint8_t *u,
                  uint8_t *v,
                  int w, int h);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    GLuint texY, texU, texV;
    int width, height;
};


