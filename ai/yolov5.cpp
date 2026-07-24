#include "yolov5.h"
#include <math.h>

#include <QElapsedTimer>
#include <QDebug>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize2.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb/stb_image_write.h"

//
// yolov5s 640大约1.1s, 320大约280ms
// yolov5n 640大约600ms, 320大约150ms

static inline float sigmoid(float x) {
    return 1.f / (1.f + exp(-x));
}

bool Yolov5::load(const char* bin, const char* param) {
    net.opt.num_threads = 4;    //强行设置4核（线程）效果明显
    net.opt.use_vulkan_compute = false;   // T507 基本不用 Vulkan 无效果
    net.opt.use_winograd_convolution = true;    // 优化：改变卷积算法，减少乘法次数，效果未知
    net.opt.use_fp16_storage = true;            // 优化：中间特征数据是否用 FP16（16位浮点）保存，效果未知
    net.opt.use_fp16_packed = false;            // T507的Cortex-A53通常不支持FP16硬件计算,无效果
    net.opt.use_packing_layout = true;          // 优化：ARM NEON可以一次处理多个数据，提高卷积计算效率，效果未知
    net.opt.use_fp16_arithmetic = false;         // 优化：无效果
    
    net.load_param(param);
    net.load_model(bin);
    return true;
}

// 把一张RGB图片 → 丢进YOLO → 得到检测框列表.
void Yolov5::detect(const unsigned char* rgb_data, int w, int h, std::vector<Object>& objects) {
    QElapsedTimer timer;    
    timer.start();

    //将原始 RGB 图像数据 rgb_data，按照宽高 w x h 缩放成 640x640(YOLO固定输入尺寸)，并创建一个 ncnn::Mat 输入张量
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(rgb_data, ncnn::Mat::PIXEL_RGB, w, h, 320, 320);   // 6ms
//    qDebug()<<"preprocess:"<<timer.elapsed() << "ms";    
//    timer.restart();    
    
    //归一化因子，表示将每个像素通道的值除以 255，即 R/255.0, G/255.0, B/255.0
    const float norm_vals[3] = {1 / 255.f, 1 / 255.f, 1 / 255.f};
    
    //对输入图像 in 做归一化处理：将 RGB 像素值从 [0, 255] 缩放到 [0, 1] 区间
    in.substract_mean_normalize(0, norm_vals);  // 1ms
//    qDebug()<<"substract:"<<timer.elapsed() << "ms";    
//    timer.restart();    

    ncnn::Extractor ex = net.create_extractor();    //从模型 net 创建一个 提取器对象
    
    ex.input("in0", in);    //将图像输入数据 in 传入模型,0ms
//    qDebug()<<"in0:"<<timer.elapsed() << "ms";    
//    timer.restart();    
    
    //从模型中提取输出
    ncnn::Mat out;  
    // yolov5s 640大约1.1s, 320大约280ms
    // yolov5n 640大约600ms, 320大约150ms
    ex.extract("out0", out);    // 若param中不是out0，请替换
    qDebug()<<"forward:"<<timer.elapsed() << "ms";    
    timer.restart();    

    
/*
    for(int i=0;i<10;i++)
    {
        const float* values = out.row(i);
    
        printf(
        "%d: v4=%f v5=%f v6=%f v83=%f v84=%f\n",
        i,
        values[4],
        values[5],
        values[6],
        values[83],
        values[84]);
    }

    for(int i=0;i<5;i++)
    {
        const float* values=out.row(i);
    
        float max=0;
        int index=-1;
    
        for(int j=4;j<85;j++)
        {
            if(values[j]>max)
            {
                max=values[j];
                index=j;
            }
        }
    
        printf("row%d max_index=%d max=%f\n",
               i,index,max);
    }

    for(int i=0;i<5;i++)
    {
        const float* values = out.row(i);
    
        printf("%d: ", i);
    
        for(int j=0;j<10;j++)
        {
            printf("%.4f ", values[j]);
        }
    
        printf("\n");
    }
*/

    // 遍历 YOLO 输出的每一行（每一个候选框）
    // out.h = 候选框数量（例如 25200）out.w = 85  out.h = 25200
//    printf("out.w = %d, out.h = %d\n", out.w, out.h);

    
    // out.h = 候选框数量，例如25200
    // out.w = 每个候选框的数据长度，例如85
    //
    // YOLOv5输出格式:
    // values[0]  : 中心点x坐标
    // values[1]  : 中心点y坐标
    // values[2]  : bbox宽度
    // values[3]  : bbox高度
    // values[4]  : objectness，表示该位置存在目标的概率
    // values[5~84] : 80个类别的概率
    //
    // 例如COCO:
    // values[5]  = person概率
    // values[16] = dog概率
    // ...
    for(int i = 0; i < out.h; i++)  // 2ms
    {
        // 获取第i个候选框的数据// values指向该候选框的85个float数据
        const float* values = out.row(i);
        
        // 第5个数据表示objectness// 表示这个框里面有没有目标
        float obj_score = values[4];    
    
        // 保存最大类别概率对应的类别ID
        int class_id = -1;
    
        // 保存最大类别概率
        float class_score = 0.f;    
    
        // 遍历80个类别
        // values[5] ~ values[84]
        // 找出概率最大的类别
        for(int j = 5; j < 85; j++)
        {
            if(values[j] > class_score)
            {
                // 当前类别概率更高
                class_score = values[j];
    
                // 类别ID
                // 因为类别从values[5]开始
                // 所以减5得到0~79
                class_id = j - 5;
            }
        }
        
        // YOLOv5最终置信度计算:
        // 目标存在概率 × 类别概率
        // 例如:
        // obj_score = 0.9
        // person概率 = 0.8
        // 最终score = 0.72
        float score = obj_score * class_score;
        
        // 过滤低置信度结果
        // 小于0.25认为是假目标
        if(score < 0.45f)
            continue;
        
        // 创建一个检测目标对象
        Object obj;    
    
        // YOLO输出的bbox格式:
        // cx : 中心点x       cy : 中心点y     w  : 宽度     h  : 高度
        // 注意:
        // YOLO输出不是左上角坐标
        float cx = values[0];
        float cy = values[1];
        float w  = values[2];
        float h  = values[3];    
    
        // 转换成ncnn常用Rect格式
        // Rect需要:
        // 左上角x 左上角y 宽 高
        // 所以:
        // 左上角 = 中心点 - 宽高的一半
        obj.rect.x = cx - w / 2;
        obj.rect.y = cy - h / 2;
    
        obj.rect.width  = w;
        obj.rect.height = h;
        
        // 保存类别编号
        // 例如:
        // 0 = person
        // 16 = dog
        obj.label = class_id;    
    
        // 保存最终置信度
        obj.prob = score;
    
    
        // 加入检测结果列表
        // 后面会交给NMS:
        // 去除大量重叠框
        objects.push_back(obj);
    }
//    qDebug()<<"for:"<<timer.elapsed() << "ms";
    
}


float iou(const Box& a, const Box& b)
{
    float inter_x1 = std::max(a.x, b.x);
    float inter_y1 = std::max(a.y, b.y);
    float inter_x2 = std::min(a.x + a.width, b.x + b.width);
    float inter_y2 = std::min(a.y + a.height, b.y + b.height);

    float inter_w = std::max(0.f, inter_x2 - inter_x1);
    float inter_h = std::max(0.f, inter_y2 - inter_y1);
    float inter_area = inter_w * inter_h;

    float area_a = a.width * a.height;
    float area_b = b.width * b.height;

    return inter_area / (area_a + area_b - inter_area);
}

void Yolov5::nms(std::vector<Object>& objects, float nms_threshold)
{
    // 按置信度从大到小排序
    std::sort(objects.begin(), objects.end(), [](const Object& a, const Object& b) {
        return a.prob > b.prob;
    });

    std::vector<bool> removed(objects.size(), false);
    // 删除“同一类别中重复、重叠太多的框”，只保留最好的那个.
    for (size_t i = 0; i < objects.size(); i++) {
        if (removed[i]) continue;
        for (size_t j = i + 1; j < objects.size(); j++) {
            if (removed[j]) continue;
            if (objects[i].label == objects[j].label) {
                if (iou(objects[i].rect, objects[j].rect) > nms_threshold) {
                    removed[j] = true;  // 抑制重叠框
                }
            }
        }
    }

    // 把没被抑制的放到一个新容器
    std::vector<Object> filtered;
    for (size_t i = 0; i < objects.size(); i++) {
        if (!removed[i]) filtered.push_back(objects[i]);
    }
    // 不复制，直接把内存交出去.
    objects = std::move(filtered);
}
