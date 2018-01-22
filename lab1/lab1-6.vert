#version 150

in vec3 in_Position;
in vec3 in_Normal;
uniform mat4 myRotMatrix2;
out vec3 NormV;
void main(void)
{
	NormV = in_Normal;
	gl_Position = myRotMatrix2*vec4(in_Position, 1.0);
}
