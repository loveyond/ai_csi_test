#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "yolov5.h"
#include "FrameBuffer.h"
#include "CameraEngine.h"
#include "YoloEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    bool ai_reasoning_enable = false;
    QTimer *timer;

    FrameBuffer frameBuffer;        // 视频帧共享buf(成员指针共享，适用多个模块共享一个资源)
    
    CameraEngine cameraEngine;
    
    YoloEngine yoloEngine;

    
    int read_cpu_temp();
    void initTimer();
};
#endif // MAINWINDOW_H
