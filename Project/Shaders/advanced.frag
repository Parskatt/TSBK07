#version 400

//In
in vec3 exColor;
in vec2 texCoord;
in vec3 surf;

//Out
out vec4 out_Color;

//Uniform
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform sampler2D tex0, tex1;
uniform float t;


void main(void)
{

  diffuse_component = dot(normalize(outNormal), LightDirection);
  diffuse_component = clamp(diffuse_component, 0, 1);
	out_Color = a*vec4(exColor,1.0);
}
