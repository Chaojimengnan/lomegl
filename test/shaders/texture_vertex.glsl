#version 330 core
layout (location = 0) in vec3 pos;              // 位置
layout (location = 2) in vec2 texture_pos;      // 在纹理的位置

uniform mat4 model;         // 模型矩阵
uniform mat4 view;          // 观察矩阵
uniform mat4 projection;    // 投影矩阵

out vec2 vertex_texture_pos;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertex_texture_pos = texture_pos;
}