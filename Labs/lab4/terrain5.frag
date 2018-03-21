#version 400

out vec4 outColor;
in vec2 texCoord;
in vec3 shade;
in vec3 Position;
uniform sampler2D tex;

void main(void)
{
	double x = min(max(Position[1]-0.1,0.0),1.0);
	outColor = vec4(x,x,1.0,1.0)*texture(tex, texCoord)*vec4(shade,1.0);
}
