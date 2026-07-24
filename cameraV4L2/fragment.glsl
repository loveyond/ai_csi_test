// 这是gpu画图需要的

// 把 YUV 转 RGB

precision mediump float;

varying vec2 texCoord;

uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main()
{
    float y = texture2D(tex_y, texCoord).r;
    float u = texture2D(tex_u, texCoord).r - 0.5;
    float v = texture2D(tex_v, texCoord).r - 0.5;

    float r = y + 1.402 * v;
    float g = y - 0.344 * u - 0.714 * v;
    float b = y + 1.772 * u;

    gl_FragColor = vec4(r, g, b, 1.0);
}

