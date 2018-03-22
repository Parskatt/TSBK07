#version 400

//In
in vec3 in_Position;
in vec2 inTexCoord;

//Out
out vec2 texCoord;

//Uniform
uniform mat4 totMatrix;
uniform sampler2D texUnit;


void main(void)
{
	texCoord = inTexCoord;
	gl_Position = totMatrix*vec4(in_Position, 1.0);
}
