#version 150
in vec3 NormV;
out vec4 out_Color;
uniform mat4 myRotMatrix2;
uniform float t;
void main(void)
{
	vec4 a = myRotMatrix2*vec4(NormV,1);
	float b = max(1/2,-a[2]/a[3]);
	out_Color = vec4(b,b,b,1.0);//
}
