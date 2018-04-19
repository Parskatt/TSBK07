#include "light_sources.h"


PointLight* make_point_light(vec3 position,float constant,float linear,float quadratic,vec3 ambient,vec3 diffuse,vec3 specular){
  PointLight* pt = malloc(sizeof(PointLight));
  pt->position = position;
  pt->constant = constant;
  pt->linear = linear;
  pt->quadratic = quadratic;
  pt->ambient = ambient;
  pt->diffuse = diffuse;
  pt->specular = specular;
  return pt;
}
DirLight* make_dir_light(vec3 direction,vec3 ambient,vec3 diffuse,vec3 specular){
  DirLight* pt = malloc(sizeof(DirLight));
  pt->direction = direction;
  pt->ambient = ambient;
  pt->diffuse = diffuse;
  pt->specular = specular;
  return pt;
}


//This is ugly but it has to be done somewhere.
LightSources* lighting_hell(){
  LightSources* lights = malloc(sizeof(LightSources));
  //Pos constant, linear, quadratic, ambient, diffuse, specular
  lights->pointlights[0] = *make_point_light(SetVector(0.5,0.0,0.0),1.0,1.0,0.0,
                            SetVector(0,0.0,0.0),SetVector(0.58,0.58,0.58),SetVector(5.0,0.0,0.0));//Red light
  lights->pointlights[1] = *make_point_light(SetVector(1.0,0.0,0.0),1.0,1.0,0.0,
                            SetVector(0.0,2,0.0),SetVector(0.0,1.0,0.0),SetVector(0.0,1.0,0.0));//Green light
  lights->pointlights[2] = *make_point_light(SetVector(2.0,0.0,0.0),1.0,1.0,0.0,
                            SetVector(0.0,0.0,2),SetVector(0.0,0.0,1.0),SetVector(0.0,0.0,1.0));//Blue light
  lights->pointlights[3] = *make_point_light(SetVector(3.0,0.0,0.0),1.0,1.0,0.0,
                            SetVector(2,0.0,2),SetVector(1.0,0.0,1.0),SetVector(1.0,0.0,1.0));//Magenta light
  lights->dirlight = *make_dir_light(SetVector(1.0,1.0,1.0),SetVector(1.0,1.0,1.0),SetVector(1.0,1.0,1.0),SetVector(1.0,1.0,1.0)); //White light
  return lights;
}
LightSources* lighting_heaven(){
  LightSources* lights = malloc(sizeof(LightSources));
  return lights;
};
void apply_lighting(LightSources* lights, GLuint* shader, vec3 cam_pos){
  //Positional lights
  glUniform3fv(glGetUniformLocation(*shader, "viewPos"),1,&cam_pos.x);
  glUseProgram(*shader);
  for (int i = 0; i < 4; i++)
  {
    char buffer[64];

    sprintf(buffer, "pointLights[%i].position", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &lights->pointlights[i].position.x);

    sprintf(buffer, "pointLights[%i].constant", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), lights->pointlights[i].constant);

    sprintf(buffer, "pointLights[%i].linear", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), lights->pointlights[i].linear);

    sprintf(buffer, "pointLights[%i].qaudratic", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), lights->pointlights[i].quadratic);

    sprintf(buffer, "pointLights[%i].ambient", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &lights->pointlights[i].ambient.x);

    sprintf(buffer, "pointLights[%i].diffuse", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &lights->pointlights[i].diffuse.x);

    sprintf(buffer, "pointLights[%i].specular", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &lights->pointlights[i].specular.x);
  }
  //Directional lights
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.direction"), 1, &lights->dirlight.direction.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.ambient"), 1, &lights->dirlight.ambient.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.diffuse"), 1, &lights->dirlight.diffuse.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.specular"), 1, &lights->dirlight.specular.x);
}
