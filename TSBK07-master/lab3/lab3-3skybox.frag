#version 150
in vec2 texCoord;
out vec4 out_Color;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D texUnit;
uniform float t;


void main(void)
{
	out_Color = texture(texUnit, texCoord);
}
