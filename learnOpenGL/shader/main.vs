#version 460 core

layout (location = 0) in vec3 aPos;             // λ��
layout (location = 1) in vec3 aNormal;          // ����
layout (location = 2) in vec2 aTexCoords;       // uv

out vec2 TexCoords;                             // uv
out vec3 FragPos;                               // λ��
out vec3 Normal;                                // ����
out vec4 FragPosLightSpace;                     // ���ڹ�����ͼ�µ�λ��


uniform mat4 model;                             // λ�þ���
uniform mat3 normal_model;                      // ���ڷ��ߵ�λ�þ���
uniform mat4 view;                              // ��ͼ����
uniform mat4 projection;                        // ͸�Ӿ���
uniform mat4 lightSpaceMatrix;                  // ���ߵ�ת������

void main()
{
    TexCoords = aTexCoords;   
    FragPos = vec3(model *vec4(aPos, 1.0));
    Normal = normalize(normal_model * aNormal);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}