#version 150
in vec3 exColor;
in vec2 texCoord;
out vec4 out_Color;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D texUnit;
uniform float t;


void main(void)
{
	vec4 a = texture(texUnit, texCoord);
	out_Color = vec4(exColor,1.0);
}
