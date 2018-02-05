#version 150

in  vec3 in_Position;
uniform mat4 myMatrix;
uniform mat4 myRotMatrix;
uniform mat4 myTranslationMatrix;
uniform float t;
void main(void)
{
	gl_Position = myRotMatrix*myTranslationMatrix*vec4(in_Position, 1.0);
}
