#version 150

out vec4 out_Color;
uniform float t;
void main(void)
{
	out_Color = vec4(abs(sin(t/400.0)),0.0,1.0,1.0);//;vec4(1.0);
}