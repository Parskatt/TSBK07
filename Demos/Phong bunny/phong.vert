#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal; // Phong
out vec3 exSurface; // Phong (specular)

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	exNormal = inverse(transpose(mat3(modelviewMatrix))) * inNormal; // Phong, "fake" normal transformation

	exSurface = vec3(modelviewMatrix * vec4(inPosition, 1.0)); // Don't include projection here - we only want to go to view coordinates

	gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0); // This should include projection
}
