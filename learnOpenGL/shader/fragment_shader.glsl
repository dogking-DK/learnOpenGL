#version 330 core
struct Material
{
	sampler2D diffuse;
	vec3 specular;

	float shininess;	
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// texture samplers
uniform float vis_degree;
uniform vec3 cam_pos;
uniform Material material;
uniform Light light;


void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 camDir = normalize(cam_pos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

	float spec = pow(max(dot(camDir, reflect(-lightDir, norm)), 0.0), material.shininess);
	vec3 spectral = spec * material.specular * light.specular;

	vec3 result = spectral + diffuse + ambient;
	FragColor = vec4(result, 1.0f);
}