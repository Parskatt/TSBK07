#version 150

in vec3 in_Position;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D texUnit;
out vec3 Normal;
out vec2 texCoord;

void main(void)
{
	texCoord = inTexCoord;
	Normal = mat3(projMatrix)*mat3(mdlMatrix) * inNormal;
	gl_Position = projMatrix*mdlMatrix*vec4(in_Position, 1.0);
}
