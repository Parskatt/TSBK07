#version 150

in vec3 in_Position;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texUnit;
out vec2 texCoord;

void main(void)
{
	texCoord = inTexCoord;
	gl_Position = projMatrix*camMatrix*mdlMatrix*vec4(in_Position, 1.0);
}
