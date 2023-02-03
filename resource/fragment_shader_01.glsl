#version 330 core

out vec4 fragment_color;

in vec4 vertex_color; // 从顶点着色器传来的变量 （名称相同 类型相同）

void main()
{
    fragment_color = vertex_color;
}