#version 400

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 shade;

uniform mat4 totMatrix;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	shade = vec3(max(0,dot(normalize(inNormal), light)));
	mat3 normalMatrix1 = mat3(totMatrix);
	texCoord = inTexCoord;
	gl_Position = totMatrix * vec4(inPosition, 1.0);
}
