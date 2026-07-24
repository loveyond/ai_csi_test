#include "mainwindow.h"

#include <QApplication>
#include "CameraV4L2.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;



    w.show();
    
    return a.exec();
}
