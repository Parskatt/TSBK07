#version 400

out vec4 outColor;
in vec2 texCoord;
in vec3 shade;
in vec3 Position;
uniform sampler2D tex;

void main(void)
{
	outColor = vec4(shade,1.0);
}
