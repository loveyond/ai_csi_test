
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Frame.h"
#include <mutex>


class FrameBuffer
{

public:

    void update(const Frame& frame)
    {
        std::lock_guard<std::mutex> lock(mutex);    // c++11 互斥锁,作用域结束时自动解锁

        latest = frame;         // 复制数据，得到一帧数据
        available = true;
    }


    bool get(Frame& frame)
    {
        std::lock_guard<std::mutex> lock(mutex);


        if(!available)
            return false;


        frame = latest;         // 复制数据，

        available = false;

        return true;
    }



private:

    Frame latest;

    bool available = false;

    std::mutex mutex;

};


#endif



