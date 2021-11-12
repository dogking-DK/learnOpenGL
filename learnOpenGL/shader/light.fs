#version 460 core

out vec4 FragColor;

in vec2 TexCoords;						// uv坐标
in vec3 FragPos;						// 位置
in vec3 Normal;							// 法线

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}