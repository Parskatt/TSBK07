#version 400

//In
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

//Out
out vec2 TexCoord;
out vec3 Normal;
out vec3 SurfacePos;

//Uniform
uniform mat4 modelToWorldMatrix;
uniform mat4 worldToViewMatrix;
uniform mat4 projectionMatrix;
//uniform sampler2D tex0, tex1;


void main(void)
{
	TexCoord = inTexCoord;
	Normal = inNormal;
	SurfacePos = vec3(modelToWorldMatrix*vec4(inPosition, 1.0));
	gl_Position = projectionMatrix*worldToViewMatrix*modelToWorldMatrix*vec4(inPosition, 1.0);
}
