#version 400

//In
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

//Out
out vec2 texCoord;

//Uniform
uniform mat4 modelToWorldMatrix;
uniform mat4 worldToViewMatrix;
uniform mat4 projectionMatrix;
uniform sampler2D texUnit;


void main(void)
{
	texCoord = inTexCoord;
	gl_Position = projectionMatrix*worldToViewMatrix*modelToWorldMatrix*vec4(inPosition, 1.0);
}
