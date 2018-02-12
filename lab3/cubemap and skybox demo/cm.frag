#version 150

out vec4 outColor;
//in vec3 exNormal;
uniform samplerCube cubemap;
in vec3 reflectedView;

void main(void)
{
// Cubemap texture only:
	outColor = texture(cubemap, normalize(reflectedView));
}
