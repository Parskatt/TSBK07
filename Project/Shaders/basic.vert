#version 150

//In
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

//Out
out vec2 TexCoord;
out float shade;

//Uniform
uniform mat4 totMatrix;

void main()
{
 	vec3 light = vec3(0.58,0.58,0.58);
	shade = max(0,dot(light,normalize(inNormal)));
	TexCoord = inTexCoord;
	gl_Position = totMatrix * vec4(inPosition, 1.0);
}
