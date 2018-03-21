#version 150

in vec3 in_Position;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texUnit;
out vec3 exColor;
out vec2 texCoord;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	texCoord = inTexCoord;
	float shade;
	vec3 Normal;
	Normal = mat3(camMatrix)*mat3(mdlMatrix) * inNormal;
	shade = max(0,dot(normalize(Normal), light));
	exColor = vec3(shade);
	gl_Position = projMatrix*camMatrix*mdlMatrix*vec4(in_Position, 1.0);
}
