#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float vis_degree;
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

void main()
{
	float ambientStrength = 0.1;
	float spectralStrength = 0.3;
	vec3 ambient = ambientStrength * light_color;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_pos - FragPos);
	vec3 camDir = normalize(cam_pos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff*light_color;

	float spec = pow(max(dot(camDir, reflect(-lightDir, norm)), 0.0), 32);
	vec3 spectral = spec * spectralStrength * light_color;

	vec3 result = (spectral + diffuse + ambient) * object_color;
	FragColor = vec4(result, 1.0f);
}