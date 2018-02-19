#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 shade;
uniform sampler2D tex;

void main(void)
{
	outColor = texture(tex, texCoord)*vec4(shade,1.0);
}
