#version 460 core

layout (location = 0) in vec3 aPos;             // λ��
layout (location = 1) in vec3 aNormal;          // ����
layout (location = 2) in vec2 aTexCoords;       // uv

out vec2 TexCoords;                             // uv
out vec3 FragPos;                               // λ��
out vec3 Normal;                                // ����

uniform mat4 model;                             // λ�þ���
uniform mat4 normal_model;                      // ���ڷ��ߵ�λ�þ���
uniform mat4 view;                              // ��ͼ����
uniform mat4 projection;                        // ͸�Ӿ���

void main()
{
    TexCoords = aTexCoords;   
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model *vec4(aPos, 1.0));
    // Normal = normal_model * aNormal;
    Normal = mat3(transpose(inverse(model)))*aNormal;
}