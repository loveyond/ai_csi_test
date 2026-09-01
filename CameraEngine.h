
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
    
    int getInterval() const { return interval; }
    
    int setInterval(int date)  {  interval = date; return 0;}

private:

    CameraV4L2* camera;

    FrameBuffer* frameBuffer;

    unsigned int interval = 50;  // 这里的数值是ms, 33ms约30fps  50ms约20fps

};

#endif

