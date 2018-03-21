#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec3 exNormal; // Phong
out vec2 texCoord;
out vec3 surf;

// NY
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(camMatrix * mdlMatrix);
	exNormal = normalMatrix1 * inNormal; // Phong
	
	texCoord = inTexCoord;
	surf = inPosition;

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
