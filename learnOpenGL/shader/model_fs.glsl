#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
uniform sampler2D texture_diffuse1;
uniform vec3 cam_pos;

void main()
{    
    // float distance = 1.0 / length(cam_pos - FragPos);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * distance * 2;
    FragColor = texture(texture_diffuse1, TexCoords);
}