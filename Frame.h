
#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <stdint.h>


struct Frame
{
    std::vector<unsigned char> rgb;
    /*
        Y：width × height
        U：width/2 × height/2
        V：width/2 × height/2
    */
    std::vector<unsigned char> y;
    std::vector<unsigned char> u;
    std::vector<unsigned char> v;

    int width = 0;
    int height = 0;

    uint64_t timestamp = 0;
};


#endif



