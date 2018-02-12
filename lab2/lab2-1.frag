#version 150
in vec3 Normal;
in vec2 texCoord;
out vec4 out_Color;
uniform mat4 myRotMatrix2;
uniform float t;

void main(void)
{
	float a1 = sin(texCoord.s*30)/2;
	float b1 = sin(texCoord.t*30)/2;
	vec4 a = myRotMatrix2*vec4(Normal,1);
	float b = max(0,-a[2]/a[3]);
	out_Color = vec4(texCoord.s,texCoord.t,texCoord.s+texCoord.t,1.0);//vec4(b*a1+b,b*b1+b,b*a1+b,1.0);//
}
