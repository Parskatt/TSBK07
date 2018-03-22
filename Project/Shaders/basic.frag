#version 150

//In
in vec2 TexCoord;
in vec3 shade;

//Out
out vec4 outColor;

//Uniforms
uniform sampler2D tex;

void main()
{
	outColor = vec4(normalize(shade), 1.0);
}
