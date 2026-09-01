#include <stdio.h>
#include <stdlib.h>

#include "Shader.h"


bool Shader::load( const char* vertexFile, const char* fragmentFile)
{

    char* vertexSource = loadFile(vertexFile);

    char* fragmentSource = loadFile(fragmentFile);
    
    //第1： Shader编译(把一段 GLSL 字符串 → 编译成 GPU 能执行的 Shader 对象)    
    GLuint vs = loadShader( GL_VERTEX_SHADER, vertexSource);
    GLuint fs = loadShader( GL_FRAGMENT_SHADER, fragmentSource);

    //第2： 创建program (把已经编译好的 Vertex Shader 和 Fragment Shader 组合起来)
    program = glCreateProgram();
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);

    // 判断连接是否成功
    GLint success;    
    glGetProgramiv( program, GL_LINK_STATUS, &success);    
    printf("program link=%d\n",success);    
    if(!success)
    {
        char log[512];    
        glGetProgramInfoLog(
            program,
            sizeof(log),
            NULL,
            log);    
        printf("link error:%s\n",log);
    }


    return true;
}

char *Shader::loadFile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if(!fp)
    {
        printf("open %s failed\n", filename);
        return NULL;
    }


    fseek(fp, 0, SEEK_END);

    long size = ftell(fp);

    rewind(fp);


    char *buffer = (char *)malloc(size + 1);

    fread(buffer, 1, size, fp);

    buffer[size] = '\0';


    fclose(fp);

    return buffer;
}


GLuint Shader::loadShader( GLenum type, const char *source)
{
    GLuint shader;
    // 创建Shader对象
    shader = glCreateShader(type);
    // 把 source 里面的 GLSL 源码交给刚才创建的 Shader
    glShaderSource( shader, 1, &source, NULL );
    // 编译shader
    glCompileShader(shader);

    // 查询Shader编译结果
    GLint success;    
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success);
    printf("shader compile=%d\n",success);
    if(!success)
    {
        char log[512];
        glGetShaderInfoLog(
            shader,
            sizeof(log),
            NULL,
            log);
        printf("shader error:%s\n",log);
    }

    return shader;
}


