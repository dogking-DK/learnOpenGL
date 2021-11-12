#version 460 core

layout (location = 0) in vec3 aPos;             // 位置
layout (location = 1) in vec3 aNormal;          // 法线

out VS_OUT
{
    vec3 normal;
} vs_out;

uniform mat4 model;                             // 位置矩阵
uniform mat3 normal_model;                      
uniform mat4 view;                              // 视图矩阵

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0);

    mat3 normal_mat = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normal_mat * aNormal, 0.0)));
}