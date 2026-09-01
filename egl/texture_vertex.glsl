#version 300 es
// 必须在第一行，不能有空行
// 告诉GPU使用 OpenGL ES 3.0 GLSL语法

// 定义一个叫 position 的变量，它来自 CPU 端传入
// layout(location=0): 这个输入变量对应 OpenGL 的 0号顶点属性通道
/*
    对应着 0：
    glVertexAttribPointer(
        0,              // 第0号顶点属性，从当前VBO读取。对应:layout(location=0) in vec3 position;
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        0);
*/
layout(location=0) in vec3 position;

/*
    UV 对应着 1：
    glVertexAttribPointer(
        1,              // 第1号顶点属性，从当前VBO读取。对应:layout(location=1) in vec2 texCoord;
        2,
        GL_FLOAT,
        GL_FALSE,
        5*sizeof(float),
        (void*)(3*sizeof(float)));
*/
layout(location=1) in vec2 texCoord;

uniform mat4 MVP;

// Vertex Shader 要把一个 vec2 数据uv 传给 Fragment Shader
out vec2 uv;

void main()
{
    gl_Position = MVP * vec4(position,1.0);     // 它是 Vertex Shader 的特殊内置输出变量

    // 把顶点的纹理坐标数据传出去给Fragment Shader的uv
    uv = texCoord;
}


