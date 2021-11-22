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

in vec2 TexCoords;						// uv坐标
in vec3 FragPos;						// 位置
in vec3 Normal;							// 法线
in vec4 FragPosLightSpace;				// 光线视图下的位置

uniform Material material;				// 物体材质
uniform vec3 cam_pos;					// 相机位置

uniform sampler2D shadowMap;			// 阴影图

uniform DirLight dir_light;				// 测试用光
uniform PointLight point_light;			// 测试用光
uniform SpotLight spot_light;			// 测试用光

vec3 calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);	// 获得入射光方向
		
	float diff = max(dot(normal, -lightDir), 0.0);	// 获得漫反射系数

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
	vec3 lightDir = normalize(dis_vec);	// 光线入射方向

	float diff = max(dot(normal, -lightDir), 0.0);

	float distance = length(dis_vec);		// 光线与着色点距离
	// 光线强度衰减
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
	vec3 lightDir = normalize(dis_vec);									// 光线入射方向

	float theta = dot(lightDir, normalize(light.direction));			// 有效夹角cosine
	float epsilon = light.cutoff - light.outer_cutoff;					// 聚光灯衰减角度
	float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

	
	float diff = max(dot(normal, -lightDir), 0.0);						// 漫反射系数
	
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
	vec3 camDir = normalize(FragPos - cam_pos);				// 相机方向
	vec3 result = vec3(0.0);								// 光线累加结果

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