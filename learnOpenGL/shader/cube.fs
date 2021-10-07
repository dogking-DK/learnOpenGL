#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
uniform sampler2D texture_diffuse1;

float near = 0.1;
float far = 100.0;

float linear_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{    
    // float distance = 1.0 / length(cam_pos - FragPos);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * distance * 2;
    // float depth = linear_depth(gl_FragCoord.z) / near / far;
    // FragColor = vec4(vec3(depth), 1.0);
    FragColor = texture(texture_diffuse1, TexCoords);
}