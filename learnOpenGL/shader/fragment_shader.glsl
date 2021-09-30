#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;	
};
struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform float vis_degree;
uniform vec3 cam_pos;
uniform Material material;
uniform Light light;
uniform sampler2D emission;

void main()
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance *distance);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	// vec3 lightDir = normalize(light.direction);
	vec3 camDir = normalize(cam_pos - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoord).rgb + texture(emission, TexCoord).rgb);
	
	float spec = pow(max(dot(camDir, reflect(-lightDir, norm)), 0.0), material.shininess);
	vec3 spectral = spec * texture(material.specular, TexCoord).rgb * light.specular;
	
	vec3 result = spectral + diffuse + ambient;
	result *= attenuation;
	FragColor = vec4(result, 1.0f);
}