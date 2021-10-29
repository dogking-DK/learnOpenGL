#version 460 core



struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct Material
{
	sampler2D texture_diffuse1;	
	sampler2D texture_specular1;
	float shininess;	
};
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);

out vec4 FragColor;

in vec2 TexCoords;						// uv����
in vec3 FragPos;						// λ��
in vec3 Normal;							// ����

uniform Material material;				// �������
uniform vec3 cam_pos;					// ���λ��
uniform DirLight temp_light;			// �����ù�
// uniform sampler2D temp;
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);	// ��ù�һ���ߵķ�����

	float diff = max(dot(normal, lightDir), 0.0);	// ���������ϵ��

	vec3 reflectLightDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(reflectLightDir, viewDir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	return (ambient + diffuse + specular);
}


void main()
{
	vec3 camDir = normalize(cam_pos - FragPos);				// �������
	vec3 result = vec3(0.0);								// �����ۼӽ��

	result += calcDirLight(temp_light, Normal, camDir);
	
	FragColor = vec4(result, 1.0);
}