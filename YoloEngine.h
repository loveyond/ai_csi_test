
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

    // const 引用：只读（限制外部不能通过返回值修改数据），第二个const：const成员函数（限制函数内部不会修改对象的任何成员变量）
    const std::vector<std::string>& getReasoningResults() const {
        return reasoning_results;
    }
    
private:
    Yolov5 yolov5;

    std::vector<Object> objects;

    std::vector<std::string> reasoning_results;
    
};


#endif

