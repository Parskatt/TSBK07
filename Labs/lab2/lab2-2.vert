#version 150

in vec3 in_Position;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 myRotMatrix2;
uniform sampler2D texUnit;
out vec3 Normal;
out vec2 texCoord;

void main(void)
{
	texCoord = inTexCoord;
	Normal = inNormal;
	gl_Position = myRotMatrix2*vec4(in_Position, 1.0);
}
