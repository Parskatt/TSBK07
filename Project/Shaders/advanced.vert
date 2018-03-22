#version 400

//In
in vec3 in_Position;
in vec3 inNormal;
in vec2 inTexCoord;

//Out
out vec2 texCoord;
out vec3 exColor;

//Uniform
uniform mat4 totMatrix;
uniform sampler2D tex0, tex1;


void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	texCoord = inTexCoord;
	float shade;
	vec3 Normal;
	Normal = mat3(camMatrix)*mat3(mdlMatrix) * inNormal;
	shade = max(0,dot(normalize(Normal), light));
	exColor = vec3(shade);
	gl_Position = totMatrix*vec4(in_Position, 1.0);
}
