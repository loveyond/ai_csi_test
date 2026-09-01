#version 300 es

// OpenGL ES 片元着色器（Fragment Shader，也叫像素着色器）
// 它决定每一个像素显示什么颜色

/*
	指定float精度：
		lowp       低精度
		mediump    中等精度
		highp      高精度
*/

precision mediump float;

/*
	uniform 所有顶点、所有片元共享的一份数据,它是由 C++ 程序传进来的
	比如：textureLoc = glGetUniformLocation(program,"texture0");
*/

uniform sampler2D texture0;

/*
	uniform 所有顶点、所有片元共享的一份数据。
	vec4 4个浮点数
*/
uniform vec4 color;

// 使用纹理 0 = 纯颜色    1 = RGB 2 = YUV
uniform int renderMode;

uniform sampler2D texY;
uniform sampler2D texU;
uniform sampler2D texV;


in vec2 uv;

/*
    Fragment Shader
          ↓
        fragColor
          ↓
    OpenGL图形流水线
          ↓
    屏幕

    gl_FragColor是GLSL ES 2.0的内置变量
    GLSL ES 3.0 推荐使用显式的输出变量
    
    out 表示：这是 Fragment Shader 的输出，供OpenGL流水线使用的

*/
out vec4 fragColor;


void main()
{
    /*
        texture0
           ↓
        一张图片

        uv
           ↓
        图片上的坐标

        texture(texture0,uv)
           ↓
        去这张图片的这个位置取颜色

        fragColor
           ↓
        这个 Fragment 最终显示这个颜色

    */
    if(renderMode == 0)
    {
        fragColor = color;
    }
    else if(renderMode == 1)
    {
        fragColor = texture(texture0, uv);
    }
    else if(renderMode == 2)
    {
        float Y = texture(texY, uv).r;
        float U = texture(texU, uv).r - 0.5;
        float V = texture(texV, uv).r - 0.5;
    
        float R = Y + 1.402 * V;
        float G = Y - 0.344 * U - 0.714 * V;
        float B = Y + 1.772 * U;
    
        fragColor = vec4(R, G, B, 1.0);
    }

}

