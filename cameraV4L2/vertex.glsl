// 这是gpu画图需要的

// 画一个矩形.

attribute vec4 vertexIn;		// 屏幕上的“位置坐标”.
attribute vec2 textureIn;

varying vec2 texCoord;

void main()
{
    gl_Position = vertexIn;
    texCoord = textureIn;
}

