QT       += core gui widgets		# QT 包含对应模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameraV4L2/CameraV4L2.cpp \
    cameraV4L2/image_converter.cpp \
#    cameraV4L2/VideoWidget.cpp \
	ai/yolov5.cpp \
    main.cpp \
    mainwindow.cpp \
    CameraEngine.cpp \
    YoloEngine.cpp	\
    GLWidget.cpp \
    egl/EGLManager.cpp egl/GLRenderer.cpp egl/Matrix4.cpp egl/Mesh.cpp egl/Shader.cpp egl/Sprite.cpp egl/Texture.cpp egl/YUVTexture.cpp

HEADERS += \
    cameraV4L2/CameraV4L2.h \
    cameraV4L2/image_converter.h \
#    cameraV4L2/VideoWidget.h \
	ai/yolov5.h \
    cameraV4L2/sunxi_camera_v2.h \
    cameraV4L2/sunxi_display2.h \
    mainwindow.h	\
    CameraEngine.h	\
    Frame.h			\
    FrameBuffer.h	\
    YoloEngine.h	\
    GLWidget.h		\
    egl/EGLManager.h egl/GLRenderer.h egl/Matrix4.h egl/Mesh.h egl/Shader.h egl/Sprite.h egl/Texture.h egl/YUVTexture.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    ai_csi_test_zh_CN.ts
    
INCLUDEPATH += $$PWD   # 把当前源码目录加入头文件搜索路径
INCLUDEPATH += $$PWD/cameraV4L2
INCLUDEPATH += $$PWD/ai
INCLUDEPATH += $$PWD/egl
INCLUDEPATH += $$PWD/../../ai_learn/ncnn-install/include/ncnn
INCLUDEPATH += $$PWD/../../test/t507_gpu/include

QMAKE_CXXFLAGS += -fopenmp -DEGL_FBDEV=1 -std=c++11
QMAKE_LFLAGS += -fopenmp 

LIBS += -L/home/gg/test/ai_test/ncnn/build/src -lncnn
LIBS += -L$$PWD/../../test/t507_gpu/lib
LIBS += -lEGL -lGLESv2

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


message("QT_VERSION = $$[QT_VERSION]")
message("QT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]")
message("QT_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]")
