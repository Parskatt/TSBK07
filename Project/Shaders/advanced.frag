#version 400

//In
in vec2 TexCoord;
in vec3 Normal;
in vec3 Light;

//Out
out vec4 out_Color;

//Uniform
uniform sampler2D TexUnit;


void main(void)
{

  float diffuse_component = dot(normalize(Normal), Light);
  diffuse_component = clamp(diffuse_component, 0, 1);
	out_Color = diffuse_component*texture(TexUnit,TexCoord);
}
