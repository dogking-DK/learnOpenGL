#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;
uniform mat4 transform;
uniform mat3 normal_model;
void main()
{
	// mat3 temp;
	// temp[0] = vec3(1.0, 0.0, 0.0);
	// temp[1] = vec3(0.0, 1.0, 0.0);
	// temp[2] = vec3(0.0, 0.0, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}