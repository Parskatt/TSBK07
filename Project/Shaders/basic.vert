#version 400
//In
in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

//Out
out vec2 texCoord;
out vec3 shade;
out  vec3 outNormal;

//Uniforms
uniform mat4 totMatrix;

void main()
{
	texCoord = inTexCoord;
	outNormal = inNormal;
	shade = vec3(1.0,1.0,1.0);
	gl_Position = totMatrix * vec4(inPosition, 1.0);
}
