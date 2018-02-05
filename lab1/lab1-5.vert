#version 150

in vec3 in_Position;
in vec3 colors;
uniform mat4 myMatrix;
uniform mat4 myRotMatrix;
uniform mat4 myRotMatrix2;
uniform mat4 myTranslationMatrix;
uniform float t;
out vec3 colorV;
void main(void)
{
	colorV = colors;
	gl_Position = myRotMatrix2*myRotMatrix*vec4(in_Position, 1.0);
}
