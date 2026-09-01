#include <math.h>



#include "Matrix4.h"


Matrix4::Matrix4()
{
    for(int i=0;i<16;i++)
        m[i]=0;
}

/*
单位矩阵
    1 0 0 0
    0 1 0 0
    0 0 1 0
    0 0 0 1
*/
Matrix4 Matrix4::identity()
{
    Matrix4 result;

    result.m[0]=1;
    result.m[5]=1;
    result.m[10]=1;
    result.m[15]=1;

    return result;
}

/*
平移矩阵
    |1 0 0 x|
    |0 1 0 y|
    |0 0 1 z|
    |0 0 0 1|
*/
Matrix4 Matrix4::translate(float x,float y,float z)
{
    Matrix4 result = Matrix4::identity();

    result.m[12]=x;
    result.m[13]=y;
    result.m[14]=z;

    return result;
}

/*
缩放矩阵
|sx 0  0  0|
|0 sy  0  0|
|0  0 sz  0|
|0  0  0  1|
*/
Matrix4 Matrix4::scale(float x,float y,float z)
{
    Matrix4 result = Matrix4::identity();

    result.m[0]=x;
    result.m[5]=y;
    result.m[10]=z;

    return result;
}

/*
正交投影矩阵
    把像素坐标转换成GL坐标:
    left\right  --> x
    bottom\top  --> y
    near\far    --> z
*/
Matrix4 Matrix4::ortho(
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far)
{
    Matrix4 result = Matrix4::identity();

    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);

    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);

    return result;
}


/*
旋转矩阵

*/
Matrix4 Matrix4::rotate(float angle)
{
    Matrix4 result = Matrix4::identity();

    float rad = angle * 3.1415926f / 180.0f;

    float c = cos(rad);
    float s = sin(rad);

    result.m[0] = c;
    result.m[1] = s;

    result.m[4] = -s;
    result.m[5] = c;

    return result;
}


/*
列主序（Column-Major）矩阵乘法
    matrix4[16]矩阵存储在内存中的方式分为行主序 和 列主序。
    
    行主序：（Row-Major）内存地址（从左到右递增）：
            [ 1   2   3   4  5   6   7   8  9  10  11   12 13  14  15 16 ]
              └── 第0行 ──┘    └── 第1行 ──┘    └── 第2行 ──┘    └── 第3行 ──┘
            访问公式：m[row * cols + col]
    列主序：内存地址（从左到右递增）：
            [ 1   5   9   13  2   6  10  14  3   7  11  15  4   8  12  16 ]
              └── 第0列 ──┘     └── 第1列 ──┘    └── 第2列 ──┘    └── 第3列 ──┘
            访问公式：m[col * rows + row]

    
    左矩阵 m (取第 0 行)                右矩阵 other (取第 0 列)
    [ m[0]  m[4]  m[8]  m[12] ]   [ other[0]  _  _  _ ]
    [  _     _     _      _   ] × [ other[1]  _  _  _ ]
    [  _     _     _      _   ]   [ other[2]  _  _  _ ]
    [  _     _     _      _   ]   [ other[3]  _  _  _ ]
    
    计算过程：(左矩阵的行 乘以 右矩阵的列 )
        result.m[0] = m[0]*other[0] + m[4]*other[1] + m[8]*other[2] + m[12]*other[3]
*/
Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    Matrix4 result;


    for(int col=0; col<4; col++)
    {
        for(int row=0; row<4; row++)
        {
            result.m[col*4+row] =
                    m[0*4+row] * other.m[col*4+0] +
                    m[1*4+row] * other.m[col*4+1] +
                    m[2*4+row] * other.m[col*4+2] +
                    m[3*4+row] * other.m[col*4+3];
        }
    }


    return result;
}




