#ifndef YOLOV5_H
#define YOLOV5_H

#include <vector>
#include "net.h"

struct Box {
    float x, y, width, height;
};

struct Object {
    Box rect;   // // 目标框的位置和尺寸
    int label;  // 类别
    float prob; // 置信度（分数）
};

class Yolov5 {
public:
    bool load(const char* bin, const char* param);
    void detect(const unsigned char* rgb_data, int w, int h, std::vector<Object>& objects);
    void nms(std::vector<Object>& objects, float nms_threshold);
private:
    ncnn::Net net;
};

#endif

