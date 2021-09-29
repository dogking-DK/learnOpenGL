#version 330

out vec4 FragColor;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
    FragColor = vec4(light.diffuse + light.ambient + light.specular, 1.0);
}