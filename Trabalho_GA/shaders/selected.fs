#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(1.0,0.5,1.0, 1.0);
}
    