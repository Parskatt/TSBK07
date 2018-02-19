#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec3 exNormal;
out vec2 texCoord;

uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

/* Grid has no texture coordinates, but is -2.5 to 2.5 so I can generate it. */

void main(void)
{
	mat3 normalMatrix1 = mat3(camMatrix * mdlMatrix);
	exNormal = normalMatrix1 * inNormal; // If we want Phong
	
	texCoord = inTexCoord;
	texCoord = vec2(((inPosition + vec3(2.5, 0, 2.5))/5).xz);

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
