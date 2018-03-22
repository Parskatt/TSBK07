#version 400

//In
in vec2 texCoord;
in vec3 shade;
in vec3 Position;

//Out
out vec4 outColor;

//Uniforms
uniform sampler2D tex;

void main()
{
	outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
