#version 450 core
in vec4 ourColor
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform vec2 offset;

void main()
{
	color = texture(ourTexture1, TexCoord+offset);
}