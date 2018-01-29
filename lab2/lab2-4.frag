#version 150
in vec3 Normal;
in vec2 texCoord;
out vec4 out_Color;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D texUnit;
uniform float t;


void main(void)
{
	vec4 a = texture(texUnit, texCoord);
  vec3 b = normalize(Normal);
	out_Color = a*max(0,-b[2]);
}
