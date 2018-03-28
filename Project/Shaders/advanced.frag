#version 400

//In
in vec2 TexCoord;
in vec3 Normal;
in vec3 SurfPos;

//Out
out vec4 out_Color;

//Uniform
uniform sampler2D TexUnit;
uniform vec3 LightPos[2];
uniform vec3 LightColor[2];

void main(void)
{
  vec3 LightDir;
  vec3 diffuse_component= vec3(0,0,0);
  for(int i = 0; i < 2; i++){
    LightDir = normalize(LightPos[i]-SurfPos);
    //Multiply color with amount that hits the surface.
    diffuse_component += clamp(dot(normalize(Normal), LightDir),0,1)*LightColor[i];
  }
  diffuse_component = clamp(diffuse_component, 0, 1);
	out_Color = vec4(diffuse_component,1.0)*texture(TexUnit,TexCoord);
}
