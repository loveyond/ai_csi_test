#ifndef MESH_H
#define MESH_H

//#include <GLES2/gl2.h>
#include <GLES3/gl3.h>



/*
                应用层
                Sprite
                  |
            -----------------
            |               |
          Mesh          Texture
            |               |
            -----------------
                  |
          GLRenderer + Shader
                  |
                  |
              OpenGL ES
                  |
                  |
             EGLManager
                  |
                  |
              LCD显示
*/

// 负责画什么，是“几何模型”, Sprite 负责“大小、怎么动”，Mesh 负责“形状”
// 网格

class Mesh
{
public:

    Mesh(const float* vertices, int vertexCount, GLenum drawMode = GL_TRIANGLES);

    ~Mesh();


    void draw();


private:

    GLuint VAO;
    GLuint VBO;

    int vertexCount;
    GLenum drawMode;
};



#endif

