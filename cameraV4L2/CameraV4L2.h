
#pragma once        // 防止头文件被重复包含 
// #include <QObject>  // 使用 Qt 的信号槽 
#include <vector>
#include <string>
#include "Frame.h"

struct buffer {
	void *start[3];
	int length[3];
};


// 底层驱动封装
class CameraV4L2 {//: public QObject {
//    Q_OBJECT
public:
    CameraV4L2(int devId, int w, int h);        // devId、w、h 来指定摄像头编号和分辨率
//    CameraV4L2(int devId, int w, int h) {}  // 添加一个空的内联定义，不然 Source Insight解析不了 
    ~CameraV4L2();
    // open --> set input --> set parm
    bool openDevice();
    /*      REQBUFS         :让内核准备buf
            QUERYBUF        :询问内核buf信息
            mmap            :映射
            QBUF            :把buf交给内核做dma用
            STREAMON        :开始视频流传输(打开摄像头的开关)
    */
    // set FMT --> get FMT
    bool setFmt();
    // REQBUFS --> QUERYBUF --> mmap / USERPTR --> QBUF
    bool reqBufs();


    bool startCapture();
    bool stopCapture();
    //     DQBUF           :取出一帧图像 
    bool getFrame(Frame& frame);






private:
    int fd;
    int devId;
    int width, height;
    int mode;
    int nplanes;
    std::string devPath;
    
    struct buffer *buffers;
    unsigned int n_buffers;
    char path_name[20] = "/mnt/sdcard";
    //std::string path_name = "/mnt/sdcard";
    unsigned int req_frame_num = 8;

    bool readFrame(Frame& frame);
};




