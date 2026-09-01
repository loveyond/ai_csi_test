

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/version.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>

#include <cstring>
#include <iostream>

//#include <QImage>

#include "CameraV4L2.h"
#include "image_converter.h"

extern "C" {
#include "sunxi_display2.h"
#include "sunxi_camera_v2.h"

}

#undef bool


/*
    REQBUFS         :让内核准备buf
      ↓
    QUERYBUF        :询问内核buf信息
      ↓
    mmap            :映射
      ↓
    QBUF            :把buf交给内核做dma用
      ↓
    STREAMON        :开始视频流传输(打开摄像头的开关)
      ↓
    DQBUF           :取出一帧图像
      ↓
    QBUF
      ↓
    DQBUF
      ↓
    QBUF

*/



namespace{
    // 这里的函数和变量都只对当前文件可见.
unsigned int wdr_mode;
#define CLEAR(x) (memset(&(x), 0, sizeof(x)))
#define ROT_90 0    

}






CameraV4L2::CameraV4L2(int devId, int w, int h)
    : devId(devId), width(w), height(h),fd(-1),mode(1)
{
    devPath = "/dev/video" + std::to_string(devId);
}

CameraV4L2::~CameraV4L2()
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)) {
		printf("VIDIOC_STREAMOFF failed\n");
	} else
		printf("VIDIOC_STREAMOFF ok\n");
		
	unsigned int i, j;
	for (i = 0; i < n_buffers; ++i) {
		for (j = 0; j < nplanes; j++)
            if (buffers[i].start[j]) {
    			if (-1 == munmap(buffers[i].start[j], buffers[i].length[j])) {
    				printf("munmap error");
    			}
			}
	}
	free(buffers);
	buffers = nullptr;
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }    
}

// open --> set input --> set parm --> 
bool CameraV4L2::openDevice()
{
    struct v4l2_input inp;
    struct v4l2_streamparm parms;
    /* 选中 video 设备节点 */
    fd = ::open(devPath.c_str(), O_RDWR | O_NONBLOCK);
    if(fd < 0) {
        perror("open");
        return false;
    }

    printf("open %s fd = %d\n", devPath.c_str(), fd);


    inp.index = devId;
    /* 
        选择“摄像头输入源” 在一个 /dev/videoX 设备里，可能有多个输入源     
        如果只有一个输入源，或驱动不支持，则这行代码无效，可以注释掉
    */
    if (-1 == ioctl(fd, VIDIOC_S_INPUT, &devId)) {
        printf("VIDIOC_S_INPUT %d error!\n", inp.index);
        return false;
    }

    CLEAR(parms);
    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    parms.parm.capture.timeperframe.numerator = 1;
    parms.parm.capture.timeperframe.denominator = 20;   // 摄像头采集20fps
    parms.parm.capture.capturemode = V4L2_MODE_VIDEO;
    /* parms.parm.capture.capturemode = V4L2_MODE_IMAGE; */
    /*when different video have the same sensor source, 1:use sensor current win, 0:find the nearest win*/
    parms.parm.capture.reserved[0] = 0;
    parms.parm.capture.reserved[1] = wdr_mode;/*2:command, 1: wdr, 0: normal*/
    /*
        设置“采集工作参数”
    */
    if (-1 == ioctl(fd, VIDIOC_S_PARM, &parms)) {
        printf("VIDIOC_S_PARM error\n");
        return false;
    }
    return true;

}

// set FMT --> get FMT -->
bool CameraV4L2::setFmt()
{
	struct v4l2_format fmt;

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	fmt.fmt.pix_mp.width = width;
	fmt.fmt.pix_mp.height = height;
	switch (mode) {
	case 0:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_SBGGR8;
		break;
	case 1:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_YUV420M;
		break;
	case 2:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_YUV420;
		break;
	case 3:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
		break;
	case 4:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_SBGGR10;
		break;
	case 5:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_SBGGR12;
		break;
	case 6:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_FBC;
		break;
	default:
		fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_YUV420M;
		break;
	}
	fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
    /* 设置视频格式（请求） */
	if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)) {
		printf("VIDIOC_S_FMT error!\n");
		return false;
	}
    /* 
        获取最终生效的格式 
        因为很多 ISP / CSI 驱动会自动“修正参数”,所以 S_FMT和G_FMT必须配对使用
    */
	if (-1 == ioctl(fd, VIDIOC_G_FMT, &fmt)) {
		printf("VIDIOC_G_FMT error!\n");
		return false;
	} else {
		nplanes = fmt.fmt.pix_mp.num_planes;
		// resolution got from sensor = 1280*720 num_planes = 3
		printf("resolution got from sensor = %d*%d num_planes = %d\n",
		       fmt.fmt.pix_mp.width, fmt.fmt.pix_mp.height,
		       fmt.fmt.pix_mp.num_planes);
	}
	return true;

}

// REQBUFS --> QUERYBUF --> mmap / USERPTR --> QBUF
bool CameraV4L2::reqBufs()
{
    unsigned int i;
    struct v4l2_requestbuffers req;
    struct v4l2_exportbuffer exp;

    CLEAR(req);
    req.count = req_frame_num;// 8 ring buffer,/* 8是常见经验值 */
    /* 多平面视频采集 */
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;
    /*
        向摄像头驱动申请8个存视频帧buffer，这是内核态
    */
    if (-1 == ioctl(fd, VIDIOC_REQBUFS, &req)) {
        printf("VIDIOC_REQBUFS error\n");
        return false;
    }
    /*
        在用户态创建一个buffer管理数组
        struct buffer {
            void *start[3];
            int length[3];
        };
        由于buffers是结构体数组指针。所以：buffers[0] buffers[1] 代表两个完全独立的struct buffer
        
        buffers[0].start[0] = mmap(得到的第0帧Y平面)
                  .start[1] = mmap(第0帧U平面)
                  .start[2] = mmap(第0帧V平面)
        buffers[1]
        buffers[2]
        ...
        buffers[7]        
    */
    buffers = static_cast<buffer*> ( calloc(req.count, sizeof(*buffers)) );

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        /* 查询第 n 个 buffer */
        /* 这里 buf 不是数据 buffer，而是查询用结构体,而这个结构体仅仅供查询用 */
        buf.index = n_buffers;
        /* 这个 buffer 有 n 个 plane */
        buf.length = nplanes;
        /* 
            创建每个 buffer 的多个 plane 描述数组,plane内容由驱动返回填充 
            plane[0] → Y
            plane[1] → U
            plane[2] → V
        */
        buf.m.planes =
            (struct v4l2_plane *)calloc(nplanes,
                        sizeof(struct v4l2_plane));
        if (buf.m.planes == NULL) {
            printf("buf.m.planes calloc failed!\n");
            return false;
        }
        /*
            向驱动查询第 n 个 buffer 的真实信息
            驱动返回：每个 plane 的信息、buffer 的真实大小、mmap 所需 offset
            目标是把“第 n 个 buffer”从 kernel 描述转换成 user 可 mmap 的信息
        */
        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) {
            printf("VIDIOC_QUERYBUF error\n");
            free(buf.m.planes);
            return false;
        }

        for (i = 0; i < nplanes; i++) {
            buffers[n_buffers].length[i] = buf.m.planes[i].length;
            /* 把 kernel buffer 直接映射到 user space */
            buffers[n_buffers].start[i] =
                mmap(NULL,/* start anywhere */
                 buf.m.planes[i].length,
                 PROT_READ | PROT_WRITE,/* required */
                 MAP_SHARED, /* recommended user 和 kernel 共享同一块内存 */
                 fd, buf.m.planes[i].m.mem_offset);

            if (buffers[n_buffers].start[i] == MAP_FAILED) {
                printf("mmap failed\n");
                free(buf.m.planes);
                return false;
            }
#if 0
            CLEAR(exp);
            exp.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
            exp.index = n_buffers;
            exp.plane = i;
            exp.flags = O_CLOEXEC;
            if (-1 == ioctl(fd, VIDIOC_EXPBUF, &exp)) {
                printf("VIDIOC_EXPBUF error\n");
                return -1;
            }
            printf("buffer %d plane %d DMABUF fd is %d\n", n_buffers, i, exp.fd);
#endif
        }
        free(buf.m.planes);
    }
    for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;
        /*
            驱动在 REQBUFS + QUERYBUF + MMAP 阶段就已经建立好了buffers的映射关系。
            QBUF 时你只需要告诉buf.index = i
            驱动就知道是 buffers[i]
            这里的buf仅仅用来告诉驱动 i的。
        */
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.length = nplanes;
        buf.m.planes =
            (struct v4l2_plane *)calloc(nplanes,
                        sizeof(struct v4l2_plane));
        /* 把 buffers[i] 加入“采集队列” */
        /* QBUF == Queue Buffer */
        if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {
            printf("VIDIOC_QBUF failed\n");
            free(buf.m.planes);
            return false;
        }
        free(buf.m.planes);
    }
    return true;
    
}


bool CameraV4L2::startCapture()
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (-1 == ioctl(fd, VIDIOC_STREAMON, &type)) {
		printf("VIDIOC_STREAMON failed\n");
		return false;
	} else
		printf("VIDIOC_STREAMON ok\n");
    return true;
}

bool CameraV4L2::stopCapture()
{
    bool ret = true;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)) {
		printf("VIDIOC_STREAMOFF failed\n");
		ret = false;
	} else
		printf("VIDIOC_STREAMOFF ok\n");
		
    return ret;
}

// select --> DQBUF --> QBUF...
bool CameraV4L2::getFrame(Frame& frame)//std::vector<unsigned char>& rgb, int& w, int& h)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    int count = 1;
    while (count-- > 0) {
        for (;;) {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);      /* 清空 fd 集合 */
            FD_SET(fd, &fds);   /* 把摄像头fd加入监听 */

            tv.tv_sec = 2; /* Timeout. */
            tv.tv_usec = 0;

            /* select等待视频帧 */
            r = select(fd + 1, &fds, NULL, NULL, &tv);

            if (-1 == r) {
                if (errno == EINTR)
                    continue;
                printf("select err\n");
            }
            if (r == 0) {
                fprintf(stderr, "select timeout\n");
                continue;
            }

            if (readFrame(frame))
                break;
            else
                return false;
        }
    }

    return true;

}

static void yuv_r90(char *dst, char *src, int width, int height)
{
	int i = 0, j = 0;

	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++)
			*(char *)(dst + j + i * height) = *(char *)(src + (height - j - 1) * width + i);
	}
}

static void uv_r90(char *dst, char *src, int width, int height)
{
	int i = 0, j = 0;

	for (i = 0; i < width/2; i++) {
		for (j = 0; j < height/2; j++)
			*(char *)(dst + j * 2 + i * height) = *(char *)(src + (height/2 - j - 1) * width + i * 2);
	}

	for (i = 0; i < width/2; i++) {
		for (j = 0; j < height/2; j++)
			*(char *)(dst + j * 2 + 1 + i * height) = *(char *)(src + (height/2 - j - 1) * width + i * 2 + 1);
	}
}


// YUV420M (multi-plane)
bool CameraV4L2::readFrame(Frame& frame)//std::vector<unsigned char>& rgb, int& w, int& h)
{
	struct v4l2_buffer buf;
	char fdstr[50];
	FILE *file_fd = NULL;
	char *dst = NULL;

	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.length = nplanes;
	buf.m.planes =
	    (struct v4l2_plane *)calloc(nplanes, sizeof(struct v4l2_plane));
    /* 从驱动队列里取出一帧已经采集好的图像 buffer            
        DQBUF = DeQueue Buffer（出队缓冲区） */
	if (-1 == ioctl(fd, VIDIOC_DQBUF, &buf)) {
		free(buf.m.planes);
		printf("VIDIOC_DQBUF failed\n");
		return false;
	}
    /* 确保驱动返回的 buffer 索引没有越界 */
	//assert(buf.index < n_buffers);
    if (buf.index >= n_buffers) {
        printf("invalid buffer index: %d\n", buf.index);
        return false;
    }

//	printf("file length = %d %d %d\n", buffers[buf.index].length[0],
//	       buffers[buf.index].length[1],
//	       buffers[buf.index].length[2]);
//	printf("file start = %p %p %p\n", buffers[buf.index].start[0],
//	       buffers[buf.index].start[1],
//	       buffers[buf.index].start[2]);

    /*YUV to QImage*/
    unsigned char* y = static_cast<unsigned char*>( buffers[buf.index].start[0] );
    unsigned char* u = static_cast<unsigned char*>( buffers[buf.index].start[1] );
    unsigned char* v = static_cast<unsigned char*>( buffers[buf.index].start[2] );

//    if (frame.rgb.size() != width * height * 3)
//        frame.rgb.resize(width * height * 3);
//
//    ImageConverter::YUV420ToRGB(
//        y, u, v,
//        width, height,
//        frame.rgb.data()
//        );
        
    if(frame.y.size() != width * height)
        frame.y.resize(width * height);
    
    if(frame.u.size() != width / 2 * height / 2)
        frame.u.resize(width / 2 * height / 2);
    
    if(frame.v.size() != width / 2 * height / 2)
        frame.v.resize(width / 2 * height / 2);
    
    memcpy(frame.y.data(), y, width * height);
    memcpy(frame.u.data(), u, width / 2 * height / 2);
    memcpy(frame.v.data(), v, width / 2 * height / 2);

    frame.width = width;
    frame.height = height;
    
//    out = QImage(rgb.data(),
//                 width,
//                 height,
//                 QImage::Format_RGB888);    
//    out = QImage(rgb.data(),
//                 width,
//                 height,
//                 QImage::Format_RGB888).copy();    


	switch (nplanes) {
	case 1:
		sprintf(fdstr, "%s/fb%d_y%d_%d_%d.bin", path_name, devId, mode, width, height);
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[0], buffers[buf.index].length[0], 1, file_fd);
		fclose(file_fd);
		break;
	case 2:
#if ROT_90
		dst = (char *)malloc(buffers[buf.index].length[0]);
		yuv_r90(dst, buffers[buf.index].start[0], width, height);
		sprintf(fdstr, "%s/fb%d_y%d_%d_%d.bin", path_name, devId, mode, height, width);
		file_fd = fopen(fdstr, "w");
		fwrite(dst, buffers[buf.index].length[0], 1, file_fd);
		fclose(file_fd);
		free(dst);

		dst = (char *)malloc(buffers[buf.index].length[1]);
		uv_r90(dst, buffers[buf.index].start[1], width, height);
		sprintf(fdstr, "%s/fb%d_uv%d_%d_%d.bin", path_name, devId, mode, height, width);
		file_fd = fopen(fdstr, "w");
		fwrite(dst, buffers[buf.index].length[1], 1, file_fd);
		fclose(file_fd);
		free(dst);
#else
		sprintf(fdstr, "%s/fb%d_y%d_%d_%d.bin", path_name, devId, mode, width, height);
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[0], buffers[buf.index].length[0], 1, file_fd);
		fclose(file_fd);
		sprintf(fdstr, "%s/fb%d_uv%d_%d_%d.bin", path_name, devId, mode, width, height);
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[1], buffers[buf.index].length[1], 1, file_fd);
		fclose(file_fd);
#endif
		break;
	case 3:
	    break;  /* 这里不生成文件 */
#if ROT_90
		dst = (char *)malloc(buffers[buf.index].length[0]);
		yuv_r90(dst, buffers[buf.index].start[0], width, height);
		sprintf(fdstr, "%s/fb%d_y%d_%d_%d.bin", path_name, devId, mode, height, width);
		file_fd = fopen(fdstr, "w");
		fwrite(dst, buffers[buf.index].length[0], 1, file_fd);
		fclose(file_fd);
		free(dst);

		dst = (char *)malloc(buffers[buf.index].length[1]);
		yuv_r90(dst, buffers[buf.index].start[1], width/2, height/2);
		sprintf(fdstr, "%s/fb%d_u%d_%d_%d.bin", path_name, devId, mode, height, width);
		file_fd = fopen(fdstr, "w");
		fwrite(dst, buffers[buf.index].length[1], 1, file_fd);
		fclose(file_fd);
		free(dst);

		dst = (char *)malloc(buffers[buf.index].length[2]);
		yuv_r90(dst, buffers[buf.index].start[2], width/2, height/2);
		sprintf(fdstr, "%s/fb%d_v%d_%d_%d.bin", path_name, devId, mode, height, width);
		file_fd = fopen(fdstr, "w");
		fwrite(dst, buffers[buf.index].length[2], 1, file_fd);
		fclose(file_fd);
		free(dst);
#else
		sprintf(fdstr, "%s/fb%d_y%d_%d_%d.bin", path_name, devId, mode, width, height);  // fb2_y1_1280_720.bin
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[0], buffers[buf.index].length[0], 1, file_fd);
		fclose(file_fd);

		sprintf(fdstr, "%s/fb%d_u%d_%d_%d.bin", path_name, devId, mode, width, height);  // fb2_u1_1280_720.bin 
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[1], buffers[buf.index].length[1], 1, file_fd);
		fclose(file_fd);

		sprintf(fdstr, "%s/fb%d_v%d_%d_%d.bin", path_name, devId, mode, width, height);  // fb2_v1_1280_720.bin
		file_fd = fopen(fdstr, "w");
		fwrite(buffers[buf.index].start[2], buffers[buf.index].length[2], 1, file_fd);
		fclose(file_fd);
#endif
		break;
	default:
		break;
	}

    /* 把刚刚用完的 buffer 重新交还给驱动，让它继续装下一帧数据 */
	if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {
		printf("VIDIOC_QBUF buf.index %d failed\n", buf.index);
		free(buf.m.planes);
		return false;
	}

	free(buf.m.planes);

	return true;
}



static void terminate(int sig_no)
{
	printf("Got signal %d, exiting ...\n", sig_no);
//	disp_disable();
	usleep(20*1000);
	exit(1);
}

static void install_sig_handler(void)
{
	signal(SIGBUS, terminate);
	signal(SIGFPE, terminate);
	signal(SIGHUP, terminate);
	signal(SIGILL, terminate);
	signal(SIGKILL, terminate);
	signal(SIGINT, terminate);
	signal(SIGIOT, terminate);
	signal(SIGPIPE, terminate);
	signal(SIGQUIT, terminate);
	signal(SIGSEGV, terminate);
	signal(SIGSYS, terminate);
	signal(SIGTERM, terminate);
	signal(SIGTRAP, terminate);
	signal(SIGUSR1, terminate);
	signal(SIGUSR2, terminate);
}




