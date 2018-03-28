#version 150

//In
in vec2 TexCoord;
in float shade;

//Out
out vec4 outColor;

//Uniforms
uniform sampler2D texUnit;

void main()
{
	outColor = shade*texture(texUnit, TexCoord);
}
