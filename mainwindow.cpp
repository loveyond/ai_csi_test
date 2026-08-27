#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QFile>


/*
qt显示流程：摄像头   ->         YUV->       RGB memcpy->    QImage->    QLabel::setPixmap()->   Qt软件绘制
    摄像头输出YUV420数据，但需要转换成qt需要的RGB数据，并放入QImage对象中，然后由软件绘制

*/

/*
    程序架构：
                    MainWindow
                         |
        --------------------------------
        |              |               |
        ↓              ↓               ↓

  CameraEngine    YoloEngine     FrameBuffer


        |
        ↓

   CameraV4L2
        |
        ↓

     V4L2设备
    /dev/video2



CameraEngine
        |
        ↓
   FrameBuffer
        |
        ↓
   YoloEngine
        |
        ↓
  Qt显示(label)

*/

/*
OpenGL ES：    Open Graphics Library for Embedded Systems

    摄像头
      |
    YUV数据
      |
    上传GPU
      |
    Shader计算
      |
    GPU输出像素
      |
    屏幕
    
或者：
    C++程序

       |
       |
    OpenGL ES(如何画)

       |
       |
    EGL(画在哪里)

       |
       |
    Mali驱动(GPU Driver)

       |
       |
    GPU硬件

*/

namespace{
    // 这里的函数和变量都只对当前文件可见.
    

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cameraEngine(&frameBuffer)                // 注意这个初始化列表，把参数frameBuffer传了进去
{
    timer = new QTimer(this);

    ui->setupUi(this);
    ui->AI_reasoning_btn->setCheckable(true);
    ui->label_1->setScaledContents(true);       // 让 QLabel 控件自动缩放其显示的图片或内容，以适应控件本身的大小



//    GLWidget *glWidget;
//    glWidget = new GLWidget(this);
//    setCentralWidget(glWidget);

    
    cameraEngine.init();
    yoloEngine.init();
    
    initTimer();
    
    connect(ui->AI_reasoning_btn, &QPushButton::clicked, this, [=](bool checked){    
                ai_reasoning_enable = checked;
                
                if(ai_reasoning_enable) cameraEngine.setFPS(160);   //yolov5s 320的用330，yolov5n 320的用160
                else cameraEngine.setFPS(50);
                
                timer->start(cameraEngine.getFPS());
                qDebug() << "AI reasoning:"
                         << ai_reasoning_enable;
            });


}

MainWindow::~MainWindow()
{
    delete ui;
}



int MainWindow::read_cpu_temp()
{
    QFile file("/sys/class/thermal/thermal_zone0/temp");

    if(file.open(QIODevice::ReadOnly))
    {
        QString str = file.readAll();

        return str.toInt() / 1000;
    }

    return -1;
}


void MainWindow::updateDisplay() {
    const auto& results = yoloEngine.getReasoningResults();

    QString resultText;
    for (const auto& str : results) {
        resultText += QString::fromStdString(str) + "\n";
    }

    QString displayText = QString("CPU温度: %1°C\n\n识别结果:\n%2")
                              .arg(read_cpu_temp())
                              .arg(resultText);

    ui->text_label2->setText(displayText);
}


void MainWindow::initTimer()
{
    connect(timer, &QTimer::timeout, this, [=](){
        static int skip = 0;
        skip++;

        cameraEngine.capture();
        Frame frame;                            // 这里的frame用于和ai解耦
        if(frameBuffer.get(frame))
        {
            if(ai_reasoning_enable){
                yoloEngine.process(frame);
                
                if(skip % 3 == 0)
                    updateDisplay();
            }
            else {
                if(skip % 20 == 0)
                    updateDisplay();
            }
            
            QImage img(
                frame.rgb.data(),
                frame.width,
                frame.height,
                frame.width*3,
                QImage::Format_RGB888
            );

            ui->label_1->setPixmap(
                QPixmap::fromImage(img)//.copy())
            );
        }

    });

    timer->start(cameraEngine.getFPS());     

}



//GLWidget::GLWidget(QWidget *parent)
//    : QOpenGLWidget(parent)
//{
//
//}
//
//
//void GLWidget::initializeGL()
//{
//    initializeOpenGLFunctions();
//
//    glClearColor(
//        1.0,
//        0.0,
//        0.0,
//        1.0
//    );
//}
//
//void GLWidget::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//}


