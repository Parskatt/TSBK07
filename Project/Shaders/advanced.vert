#version 400

//In
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
//in vec3 LightDirection;

//Out
out vec2 TexCoord;
out vec3 Normal;
out vec3 Light;

//Uniform
uniform mat4 totMatrix;
uniform sampler2D tex0, tex1;


void main(void)
{
	TexCoord = inTexCoord;
	Normal = inNormal;
	Light = vec3(0.58, 0.58, 0.58);
	gl_Position = totMatrix*vec4(inPosition, 1.0);
}
