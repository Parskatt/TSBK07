#include "light_sources.h"
PointLight* make_point_light(vec3 position,float constant,float linear,float quadratic,vec3 ambient,vec3 diffuse,vec3 specular){
  PointLight* pt;
  pt->position = position;
  pt->constant = constant;
  pt->linear = linear;
  pt->quadratic = quadratic;
  pt->ambient = ambient;
  pt->diffuse = diffuse;
  pt->specular = specular;
  return pt;
}


//This is ugly but it has to be done somewhere. (If you're really ambitious you could place this in a file and read from file)
LightSources* lighting_hell(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {1.0f, 0.0f, 0.0f}, // Red light, positional
                              {1.0f, 1.0f, 0.0f}};
  lights->attenuation = {0.5,0.01};
  return lights;
}

LightSources* lighting_heaven(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {0.0f, 0.0f, 1.0f}, // Blue light, positional
                              {1.0f, 1.0f, 1.0f}}; //White light
  lights->attenuation = {0.5,0.01};
  return lights;
};
void apply_lighting(LightSources* Lights, GLuint* shader){
  glUniform3fv(glGetUniformLocation(*shader, "LightPos"), 2, &LightPos[0].x);
	glUniform3fv(glGetUniformLocation(*shader, "LightColor"), 2, &LightColor[0].x);
  for (int i = 0; i < 4; i++)
{
  char buffer[64];

  sprintf(buffer, "pointLights[%i].position", i);
  setUniformVec3(lightingShader, buffer, &Lights->PointLights[i].position.x); //Lul

  sprintf(buffer, "pointLights[%i].constant", i);
  setUniformFloat(lightingShader, buffer, 1.0f);

  sprintf(buffer, "pointLights[%i].linear", i);
  setUniformFloat(lightingShader, buffer, 0.09f);

  sprintf(buffer, "pointLights[%i].qaudratic", i);
  setUniformFloat(lightingShader, buffer, 0.032f);

  sprintf(buffer, "pointLights[%i].diffuse", i);
  setUniformVec3(lightingShader, buffer, pointLightColors[i]);

  sprintf(buffer, "pointLights[%i].specular", i);
  setUniformVec3(lightingShader, buffer, glm::vec3(0.0f));

  sprintf(buffer, "pointLights[%i].specular", i);
  setUniformVec3(lightingShader, buffer, glm::vec3(1.0f));
}
}
