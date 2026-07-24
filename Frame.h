
#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <stdint.h>


struct Frame
{
    std::vector<unsigned char> rgb;

    int width = 0;
    int height = 0;

    uint64_t timestamp = 0;
};


#endif



