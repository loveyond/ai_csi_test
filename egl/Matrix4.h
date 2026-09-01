
#ifndef MATRIX4_H
#define MATRIX4_H


class Matrix4
{
public:

    float m[16];


    Matrix4();


    static Matrix4 identity();      // 单位矩阵


    static Matrix4 translate(float x, float y, float z);    // 平移矩阵


    static Matrix4 scale(float x, float y, float z);    // 缩放矩阵

    static Matrix4 ortho(  // 正交投影矩阵
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far
    );

    static Matrix4 rotate(float angle); // 旋转矩阵



    // 第1个const: 接收一个 const 引用的右操作数（只读）
    // 第2个const: 不修改左操作数（*this 是只读的）
    Matrix4 operator*(const Matrix4& other) const;


};


#endif




