#version 460 core

#define SAMPLE_NUM 25
#define PI 3.14159265359
#define PI2 6.28318530718
#define BIAS 0.005
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
float rand(float seed);
float to0_1(float x);
float to1_1(float x);
void rand_sample(const in vec2 point);
float search_block(vec2 loc, float cur_depth);
float PCSS(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

float ShadowCalculation(vec4 FragLightSpace);
float avg_block_depth(vec4 FragLightSpace, float w_light);


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

vec2 circle_samples[SAMPLE_NUM];
float bias = 0.0;

// 随机数生成器
float rand(float seed)
{
	return mod((1664525.0 * seed + 1013904223.0), (4294967296.0));
}
// 将随机数缩小至0 - 1
float to0_1(float x)
{
	return (sin(x * 10000.0) + 1.0) / 2.0;
}
// 将随机数缩小至-1 - 1
float to1_1(float x)
{
	return sin(x * 10000.0);
}
int xorshift(int x)
{
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;

	return x;
}

// 在半径为1的圆中随机采样SAMPLE_NUM个点，储存在circle_samples中s
void rand_sample(const in vec2 point)
{
	float angle = to1_1(rand(point.x * 10000.0 + point.y * 10000.0)) * PI2;
	float radius = to0_1(rand(point.x * 10000.0 + point.y * 1000.0));

	for (uint i = 0; i < SAMPLE_NUM; ++i)
	{
		circle_samples[i] = vec2(radius * cos(angle), radius * sin(angle));

		angle = to1_1(rand(radius * 10000.0)) * PI2;
		radius = to0_1(rand(angle * 10000.0));
	}
}
void possion_disk_sample(const in vec2 point)
{
	float ANGLE_STEP = PI2 * float( 10 ) / float( SAMPLE_NUM );
	float INV_NUM_SAMPLES = 1.0 / float( SAMPLE_NUM );

	float angle = to0_1(rand(point.x * 10000.0 + point.y * 10000.0)) * PI2;
	float radius = INV_NUM_SAMPLES;
	float radiusStep = radius;

	for( int i = 0; i < SAMPLE_NUM; i ++ ) 
	{
		circle_samples[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
		radius += radiusStep;
		angle += ANGLE_STEP;
	}
}
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

	float shadow = PCSS(FragPosLightSpace, normal, lightDir);
	// float shadow = ShadowCalculation(FragPosLightSpace);
	// return vec3(1.0 - shadow);
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

float w_penumbra(float d_receiver, float d_blocker, float w_light) {
    return (d_receiver - d_blocker) * w_light/ d_blocker;
}
float search_block(vec2 loc, float cur_depth)
{
	float block_depth = 0.0;
	float temp_depth = 0.0;
	int valid_depth_count = 0;
	possion_disk_sample(TexCoords * 10000);

	for (uint i = 0; i < SAMPLE_NUM; ++i)
	{
		temp_depth = texture(shadowMap,loc + circle_samples[i] * 0.01).r;
		if (cur_depth - bias > temp_depth)
			{
				valid_depth_count += 1;
				block_depth += temp_depth;
			}
	}
	if (valid_depth_count == 0)
		return 0;

	return block_depth / valid_depth_count;
}

float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	if (currentDepth > 1.0)
		return 0.0;

	float shadow = (currentDepth - BIAS) > closestDepth ? 1.0 : 0.0;
	// return closestDepth;
	return shadow;
}

float avg_block_depth(vec4 FragLightSpace, float w_light){
    int sampleCount = 8;
    float blockerSum = 0;
    int blockerCount = 0;
    //float w_light = 5;
    
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    vec3 projCoords = FragLightSpace.xyz / FragLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    //float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    float search_range = w_light * (currentDepth - 0.05) /currentDepth;
    if(search_range <= 0){
        return 0;
    }
    //return search_range / 7.0;
    int range = int(search_range);
    //return range / 10.0;
    
    int window = 3;
    for( int i = -window; i < window; ++i ){
        for( int j = -window; j < window; ++j ){
            vec2 shift = vec2(i * 1.0 * range / window, j * 1.0 * range / window);
            
            //sampleDepth = shadow map value at location (i , j) in the search region
            float sampleDepth = texture(shadowMap, projCoords.xy + shift * texelSize).r;
            
            if ( sampleDepth < currentDepth ){
                blockerSum += sampleDepth;
                blockerCount++;
            }
        }
    }
    
    if(blockerCount > 0){
        return blockerSum / blockerCount;
    } else {
        return 0; //--> not in shadow~~~~
    }
}
float ShadowCalculation(vec4 FragLightSpace)
{
    // perform perspective divide
    vec3 projCoords = FragLightSpace.xyz / FragLightSpace.w;
    
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(point_light.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    
    float w_light = 7.0;
    
    float avg_depth = avg_block_depth(FragLightSpace, w_light);
    //return  avg_depth;
    if(avg_depth == 0){
        return 0.0f;
    }
    
    float w_penumbra = w_penumbra(currentDepth, avg_depth, w_light);
    // return w_penumbra;
    
    //if penumbra is zero, it has solid shadow - dark
    if(w_penumbra == 0){
        return 0;
    }
    
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int count = 0;
    
    //pick a searching range based on calculated penumbra width.
    int range = int(w_penumbra/0.09);
    
    //manually set a range if it is too large!
    range = range > 40 ? 40 : range;
    range = range < 1 ? 2 : range;
    
    //return w_penumbra;
    
    for(int x = -range; x <= range; ++x)
    {
        count++;
        for(int y = -range; y <= range; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    count = count > 0? count : 1;
    shadow /= (count * count);
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
        
    return shadow;
}
float PCSS(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	float currentDepth = projCoords.z;
    bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);

	float blockDepth = search_block(projCoords.xy, currentDepth);
	//float blockDepth = avg_block_depth(fragPosLightSpace, 0.1);
	if (blockDepth < bias || currentDepth > 1.0)
		return 0.0;

	// return blockDepth;
	// 获得伴影大小
	float penumbra = 0.1 * (currentDepth - blockDepth) / blockDepth;
	
	// return penumbra;

	float temp_depth = 0.0;
	float depth = 0.0;
	for (uint i = 0; i < SAMPLE_NUM; ++i)
	{
		temp_depth = texture(shadowMap, projCoords.xy + circle_samples[i] * 0.1).r;
		// depth += temp_depth;
		if (currentDepth - bias > temp_depth)
			depth += 1.0;
	}
	

	return depth / SAMPLE_NUM;
}