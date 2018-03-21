#version 150
in vec3 exColor;
in vec2 texCoord;
in vec3 surf;
out vec4 out_Color;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D tex0, tex1;
uniform float t;


void main(void)
{
	//vec4 a = texture(tex0, texCoord) * 0.5 + texture(tex1, texCoord) * 0.5;
	vec4 a = texture(tex0, texCoord) * (0.5+sin(surf.z*15 + t/500)/2) + texture(tex1, texCoord) * (0.5-sin(surf.z*15 + t/500)/2);
	out_Color = a*vec4(exColor,1.0);
}
