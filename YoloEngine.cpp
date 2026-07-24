
#include "YoloEngine.h"
// yolov5s 640大约1.1s, 320大约280ms
// yolov5n 640大约600ms, 320大约150ms

YoloEngine::YoloEngine()
{

}

bool YoloEngine::init(){
    return yolov5.load("yolov5n.bin", "yolov5n.param");
}

void YoloEngine::process(Frame& frame)
{
    objects.clear();

    yolov5.detect(
        frame.rgb.data(),
        frame.width,
        frame.height,
        objects
    );  // 4核 1135ms ，单核 3000ms, 4核320,320的只需300ms

    yolov5.nms(objects,0.45f);
    printf("ofter nms,objects size = %zu\n", objects.size()); // 容器里面有多少个元素.

    for(auto& obj:objects)
    {
        draw_box(
            frame.rgb.data(),
            frame.width,
            frame.height,
            3,
            obj
        );
    }

}
/*
static const char* class_names[] = {
    "person", "bicycle", "car", "motorcycle", "airplane",
    "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird",
    "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack",
    "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat",
    "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle",
    "wine glass", "cup", "fork", "knife", "spoon",
    "bowl", "banana", "apple", "sandwich", "orange",
    "broccoli", "carrot", "hot dog", "pizza", "donut",
    "cake", "chair", "sofa", "pottedplant", "bed",
    "diningtable", "toilet", "tvmonitor", "laptop", "mouse",
    "remote", "keyboard", "cell phone", "microwave", "oven",
    "toaster", "sink", "refrigerator", "book", "clock",
    "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
};
*/
static const char* class_names[] = {
    "人", "自行车", "汽车", "摩托车", "飞机",
    "公交车", "火车", "卡车", "船", "交通信号灯",
    "消防栓", "停车标志", "停车计时器", "长凳", "鸟",
    "猫", "狗", "马", "羊", "牛",
    "大象", "熊", "斑马", "长颈鹿", "背包",
    "雨伞", "手提包", "领带", "手提箱", "飞盘",
    "滑雪板", "滑雪板", "运动球", "风筝", "棒球棒",
    "棒球手套", "滑板", "冲浪板", "网球拍", "瓶子",
    "酒杯", "杯子", "叉子", "刀子", "勺子",
    "碗", "香蕉", "苹果", "三明治", "橙子",
    "西兰花", "胡萝卜", "热狗", "披萨", "甜甜圈",
    "蛋糕", "椅子", "沙发", "盆栽植物", "床",
    "餐桌", "马桶", "显示器", "笔记本电脑", "鼠标",
    "遥控器", "键盘", "手机", "微波炉", "烤箱",
    "烤面包机", "水槽", "冰箱", "书", "钟",
    "花瓶", "剪刀", "泰迪熊", "吹风机", "牙刷"
};


// 在RGB图像上画红色边框
void YoloEngine::draw_box(unsigned char* img, int w, int h, int channels, const Object& obj)
{
    // 坐标从 640×640 映射回原图大小 w × h
    float scale_x = (float)w / 320.0f;
    float scale_y = (float)h / 320.0f;


    int x0 = obj.rect.x * scale_x;
    int y0 = obj.rect.y * scale_y;

    int box_w = obj.rect.width * scale_x;
    int box_h = obj.rect.height * scale_y;


    int x1 = std::min(w - 1, x0 + box_w);
    int y1 = std::min(h - 1, y0 + box_h);


    x0 = std::max(0, x0);
    y0 = std::max(0, y0);

//    int x_center = (int)(obj.rect.x * scale_x);
//    int y_center = (int)(obj.rect.y * scale_y);
//    int box_w = (int)(obj.rect.width * scale_x);
//    int box_h = (int)(obj.rect.height * scale_y);
//
//    int x0 = std::max(0, x_center - box_w / 2);
//    int y0 = std::max(0, y_center - box_h / 2);
//    int x1 = std::min(w - 1, x_center + box_w / 2);
//    int y1 = std::min(h - 1, y_center + box_h / 2);

    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            if (y == y0 || y == y1 || x == x0 || x == x1) {
                int idx = (y * w + x) * channels;
                img[idx + 0] = 255; // R
                img[idx + 1] = 0;   // G
                img[idx + 2] = 0;   // B
            }
        }
    }
    printf("label=%s prob=%.3f\n",
           class_names[obj.label],
           obj.prob);
}


