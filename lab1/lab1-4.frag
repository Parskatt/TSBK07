#version 150
in vec3 colorV;
out vec4 out_Color;
uniform float t;
void main(void)
{
	out_Color = vec4(colorV,1.0);//
}
