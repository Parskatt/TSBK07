#version 150
in vec3 Normal;
in vec2 texCoord;
out vec4 out_Color;
uniform mat4 myRotMatrix2;
uniform sampler2D texUnit;
uniform float t;


void main(void)
{
	vec4 a = texture(texUnit, texCoord);
	vec4 b = myRotMatrix2*vec4(Normal,1);
	out_Color = a*max(0,-b[2]/b[3]);
}
