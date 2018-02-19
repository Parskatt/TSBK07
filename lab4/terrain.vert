#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 shade;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	shade = vec3(max(0,dot(normalize(inNormal), light)));
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
