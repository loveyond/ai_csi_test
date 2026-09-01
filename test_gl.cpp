#include <QApplication>
#include <QOpenGLWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QOpenGLWidget widget;
    widget.show();

    return app.exec();
}