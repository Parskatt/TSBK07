#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;

void main(void)
{
// Texture only:
	outColor = texture(tex, texCoord);
}
