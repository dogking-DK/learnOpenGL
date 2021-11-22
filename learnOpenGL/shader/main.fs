#version 460 core

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
struct SpotLight
{
	vec3 direction;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutoff;
	float outer_cutoff;
};
struct Material
{
	sampler2D texture_diffuse1;	
	sampler2D texture_specular1;
	float shininess;	
};
vec3 calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

out vec4 FragColor;

in vec2 TexCoords;						// uv����
in vec3 FragPos;						// λ��
in vec3 Normal;							// ����
in vec4 FragPosLightSpace;				// ������ͼ�µ�λ��

uniform Material material;				// �������
uniform vec3 cam_pos;					// ���λ��

uniform sampler2D shadowMap;			// ��Ӱͼ

uniform DirLight dir_light;				// �����ù�
uniform PointLight point_light;			// �����ù�
uniform SpotLight spot_light;			// �����ù�

vec3 calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);	// �������ⷽ��
		
	float diff = max(dot(normal, -lightDir), 0.0);	// ���������ϵ��

	vec3 halfWayDir = normalize(-viewDir - lightDir);
	float spec = pow(max(dot(halfWayDir, normal), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	float shadow = calcShadow(FragPosLightSpace, normal, lightDir);

	return (ambient + (1.0 - shadow) * (diffuse + specular));
}
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 dis_vec = FragPos - light.position;
	vec3 lightDir = normalize(dis_vec);	// �������䷽��

	float diff = max(dot(normal, -lightDir), 0.0);

	float distance = length(dis_vec);		// ��������ɫ�����
	// ����ǿ��˥��
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 halfWayDir = normalize(-viewDir - lightDir);
	float spec = pow(max(dot(halfWayDir, normal), 0.0), 128);
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	float shadow = calcShadow(FragPosLightSpace, normal, lightDir);
	
	return attenuation * (ambient + (1.0 - shadow) * (diffuse + specular));
}
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 dis_vec = FragPos - light.position;
	vec3 lightDir = normalize(dis_vec);									// �������䷽��

	float theta = dot(lightDir, normalize(light.direction));			// ��Ч�н�cosine
	float epsilon = light.cutoff - light.outer_cutoff;					// �۹��˥���Ƕ�
	float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

	
	float diff = max(dot(normal, -lightDir), 0.0);						// ������ϵ��
	
	vec3 halfWayDir = normalize(-viewDir - lightDir);
	float spec = pow(max(dot(halfWayDir, normal), 0.0), 32);
	// return vec3(1.0) * spec;
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	float shadow = calcShadow(FragPosLightSpace, normal, lightDir);

	return intensity * (specular + diffuse);
}

void main()
{
	vec3 camDir = normalize(FragPos - cam_pos);				// �������
	vec3 result = vec3(0.0);								// �����ۼӽ��

	// result += calcDirectionalLight(dir_light, Normal, camDir);
	result += calcPointLight(point_light, Normal, camDir);
	// result += calcSpotLight(spot_light, Normal, camDir);
	
	FragColor = vec4(result, 1.0);
}

float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	if (projCoords.z > 1.0)
		return 0.0;

	float bias = max(0.05*(1.0 - dot(normal, lightDir)), 0.005);

	float shadow = (currentDepth - 0.005) > closestDepth ? 1.0 : 0.0;
	// return closestDepth;
	return shadow;
}