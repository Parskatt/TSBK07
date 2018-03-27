#version 400

//In
in vec2 texCoord;

//Out
out vec4 out_Color;

//Uniforms
uniform sampler2D texUnit;


void main(void)
{
	out_Color = texture(texUnit, texCoord);
}
