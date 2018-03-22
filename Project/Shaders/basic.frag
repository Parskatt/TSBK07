#version 150

//In
in vec2 texCoord;
in vec3 shade;

//Out
out vec4 outColor;

//Uniforms
uniform sampler2D tex;

void main()
{
	outColor = texture(tex, texCoord);
}
