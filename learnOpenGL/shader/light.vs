#version 460 core

layout (location = 0) in vec3 aPos;             // 位置
layout (location = 1) in vec3 aNormal;          // 法线
layout (location = 2) in vec2 aTexCoords;       // uv

out vec2 TexCoords;                             // uv
out vec3 FragPos;                               // 位置
out vec3 Normal;                                // 法线

uniform mat4 model;                             // 位置矩阵
uniform mat4 normal_model;                      // 用于法线的位置矩阵
uniform mat4 view;                              // 试图矩阵
uniform mat4 projection;                        // 透视矩阵

void main()
{
    TexCoords = aTexCoords;   
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model *vec4(aPos, 1.0));
    // Normal = normal_model * aNormal;
    Normal = mat3(transpose(inverse(model)))*aNormal;
}