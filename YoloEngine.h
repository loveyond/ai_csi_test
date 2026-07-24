
#ifndef YOLOENGINE_H
#define YOLOENGINE_H

#include "yolov5.h"
#include "Frame.h"

class YoloEngine
{

public:

    YoloEngine();

    bool init();

    void process(Frame& frame);

    void draw_box(unsigned char* img, int w, int h, int channels, const Object& obj); //const Box& box);

private:
    Yolov5 yolov5;

    std::vector<Object> objects;
};


#endif

