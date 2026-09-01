
#include "Mesh.h"

// 负责为GPU提供数据，并告诉GPU顶点的格式（位置、UV）
// Sprite 负责“怎么动”，Mesh 负责“长什么样”
// 网格: 它和Shader的关系为：Mesh 提供“数据”，Shader 解析“数据”。
/*
    ┌────┬────┬────┬────┬────┐
    │ x  │ y  │ z  │ u  │ v  │
    └────┴────┴────┴────┴────┘
     ↑                ↑
     │                │
     │                │
    location 0      location 1
    position        texCoord
*/
Mesh::Mesh(const float* vertices, int count, GLenum drawMode)
{
    vertexCount = count;
    this->drawMode = drawMode;
    
    // 获取顶点数据vertices

    // 创建1个VAO ,Vertex Array Object，顶点数组对象
    glGenVertexArrays(1,&VAO);  // 创建一个空的 VAO
    
// 绑定 VAO，告诉 OpenGL 开始记录状态,接下来所有跟顶点相关的设置，都记到这个 VAO 里面
    glBindVertexArray(VAO);

    // 创建1个VBO(把CPU的数据上传到GPU),Vertex Buffer Object（顶点缓冲对象）
    glGenBuffers(1,&VBO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO);    // VBO 绑定到 GL_ARRAY_BUFFER
    glBufferData(
        GL_ARRAY_BUFFER,
        count * 5 * sizeof(float),
        vertices,
        GL_STATIC_DRAW);                    // 把vertices 数据copy到这个 VBO 对应的 GPU内存

    // 告诉GPU顶点格式位置
    glVertexAttribPointer(
        0,                          // 第0号顶点属性，从当前VBO读取。对应:layout(location=0) in vec3 position;
        3,                          // 每个顶点有 3 个数据
        GL_FLOAT,
        GL_FALSE,                   // 不要把这个数据额外归一化
        5 * sizeof(float),          // 从一个顶点跳到下一个顶点，需要跳多少字节? 叫 stride（步长）:5*4=20字节
        0);                         // offset（偏移量）
    glEnableVertexAttribArray(0);   // 打开 0 号顶点属性
    // UV
    glVertexAttribPointer(
        1,                          // 第1号顶点属性，从当前VBO读取。对应:layout(location=1) in vec2 texCoord;
        2,
        GL_FLOAT,
        GL_FALSE,
        5*sizeof(float),
        (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);   // 打开 1 号顶点属性
    
// 解绑 VAO，结束记录
    glBindVertexArray(0);       

}




Mesh::~Mesh()
{
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
}



void Mesh::draw()
{
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);     // 绑定VAO
    
    glDrawArrays(               // 设置
        drawMode,
        0,
        vertexCount
    );

//    glBindVertexArray(0);       // 解绑

}





