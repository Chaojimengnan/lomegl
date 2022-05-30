#version 330 core
out vec4 frag_color;

in vec2 vertex_texture_pos;         // 从顶点着色器输出的纹理位置
uniform vec4 our_color;             // 由c++程序指定
uniform sampler2D texture1;         // 特殊的uniform，包含纹理数据，它不需要C++程序指定，当调用draw函数时自动赋予
uniform sampler2D texture2;         // 特殊的uniform，包含纹理数据，它不需要C++程序指定，当调用draw函数时自动赋予

void main()
{
    // texture是GLSL内建函数，用于采样纹理的颜色(根据纹理数据和纹理坐标)
    // frag_color = texture(our_texture, vertex_texture_pos)  * vertex_color * our_color;
    // frag_color = texture(our_texture, vertex_texture_pos);
    frag_color = mix(texture(texture1, vertex_texture_pos), texture(texture2, vertex_texture_pos), our_color);
}