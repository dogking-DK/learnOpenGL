#version 460 core

out vec4 FragColor;

in vec2 TexCoords;						// uv����
in vec3 FragPos;						// λ��
in vec3 Normal;							// ����

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}