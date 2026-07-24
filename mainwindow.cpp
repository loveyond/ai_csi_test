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


void MainWindow::initTimer()
{
    connect(timer, &QTimer::timeout, this, [=](){
        static int skip = 0;
        skip++;

        cameraEngine.capture();
        Frame frame;
        if(frameBuffer.get(frame))
        {
            if(ai_reasoning_enable){
                yoloEngine.process(frame);
                
                if(skip % 3 == 0)
                    ui->text_label2->setText(
                        QString("CPU Temp: %1 C").arg(read_cpu_temp())
                    );
            }
            else {
                if(skip % 20 == 0)
                    ui->text_label2->setText(
                        QString("CPU Temp: %1 C").arg(read_cpu_temp())
                    );
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




/*    
        QElapsedTimer tmp_timer;
        tmp_timer.start();
        
        if (camera->getFrame(rgb, width, height)) {
            static int skip = 0;
            skip++;
//            qDebug() << "camera =" << tmp_timer.elapsed();  //28ms
            tmp_timer.restart();
            
            if (ai_reasoning_enable)//skip++ % 2)   // 每3帧推一次.
            {
                objects.clear();
                yolov5.detect(rgb.data(), width, height, objects);
                
                qDebug() << "detect =" << tmp_timer.elapsed() << "ms";  // 4核 1135ms ，单核 3000ms, 4核320,320的只需300ms
                tmp_timer.restart();
//                printf("objects size = %zu\n", objects.size()); // 容器里面有多少个元素.
                yolov5.nms(objects, 0.45f);

                printf("ofter nms,objects size = %zu\n", objects.size()); // 容器里面有多少个元素.
//                qDebug() << "nms =" << tmp_timer.elapsed() << "ms";
                tmp_timer.restart();
                
                for (auto& obj : objects){
                    draw_box(rgb.data(), width, height, 3, obj);// .rect);
                }    
//                qDebug() << "draw_box =" << tmp_timer.elapsed() << "ms";    //3ms
                tmp_timer.restart();

                if(skip % 3 == 0)
                    ui->text_label2->setText(
                        QString("CPU Temp: %1 C").arg(read_cpu_temp())
                    );
            }
            else {
                if(skip % 20 == 0)
                    ui->text_label2->setText(
                        QString("CPU Temp: %1 C").arg(read_cpu_temp())
                    );
            }
            
            QImage img(rgb.data(), width, height, width * 3, QImage::Format_RGB888);

//            QPixmap pixmap = QPixmap::fromImage(img);
//            ui->label_1->setPixmap(
//                pixmap.scaled(ui->label_1->size(),
//                              Qt::KeepAspectRatio,
//                              Qt::SmoothTransformation));     // 这个需要31ms
                              
            ui->label_1->setPixmap(QPixmap::fromImage(img));                  
//            qDebug() << "setPixmap =" << tmp_timer.elapsed() << "ms";   //3ms
        }
*/
    });

    timer->start(cameraEngine.getFPS());     

}


