#version 460 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D depthMap;

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	FragColor = vec4(vec3(depthValue, depthValue * depthValue, 0.0), 1.0);
	// FragColor = vec4(vec3(0.5f), 1.0);
}