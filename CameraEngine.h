
#ifndef CAMERAENGINE_H
#define CAMERAENGINE_H

#include "CameraV4L2.h"
#include "FrameBuffer.h"

class CameraEngine
{

public:

    CameraEngine(FrameBuffer* buffer);
    ~CameraEngine();

    bool init();
    
    void capture();
    
    int getFPS() const { return fps; }
    
    int setFPS(int date)  {  fps = date; return 0;}

private:

    CameraV4L2* camera;

    FrameBuffer* frameBuffer;

    unsigned int fps = 50;  // 33ms约30fps  

};

#endif

