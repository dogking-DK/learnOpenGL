#version 460 core

layout (location = 0) in vec3 aPos;             // λ��
layout (location = 1) in vec3 aNormal;          // ����

out VS_OUT
{
    vec3 normal;
} vs_out;

uniform mat4 model;                             // λ�þ���
uniform mat3 normal_model;                      
uniform mat4 view;                              // ��ͼ����

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0);

    mat3 normal_mat = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normal_mat * aNormal, 0.0)));
}