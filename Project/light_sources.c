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
  LightSources* lights = malloc(sizeof(LightSources)+1000*sizeof(PointLight)); //Size is fixed to around 1000 point lights, TODO allow for dynamic allocation using realloc

  lights->num_of_ptlights = 12;
  //Pos constant, linear, quadratic, ambient, diffuse, specular
  for(int n = 0; n < lights->num_of_ptlights; ++n){
    lights->pointlights[n] = *make_point_light(SetVector(124.0,49.0,124.0),0.1,0.2,0.8,
                              SetVector(1.0,0.4,0.0),SetVector(1.0,0.4,0.0),SetVector(1.0,0.4,0.0));//Magenta light
  }
  lights->dirlight = *make_dir_light(SetVector(0.0,-1.0,0.0),SetVector(0.15,0.15,0.15),SetVector(0.3,0.3,0.4),SetVector(0.0,0.0,0.0)); //White light

  return lights;
}

LightSources* lighting_heaven(){
  LightSources* lights = malloc(sizeof(LightSources));
  return lights;
}

LightSources* append_ptlight(LightSources* lights, PointLight* ptlight){
  lights->pointlights[lights->num_of_ptlights] = *ptlight;
  lights->num_of_ptlights = lights->num_of_ptlights+1; //TODO This is unsafe if too many lights are added
  return lights;
}

void sort_lights(LightSources* lights, vec3 cam_pos){
  int n = lights->num_of_ptlights;
  for(int curr = 1; curr < n; ++curr){
    int cmp = curr -1;
    PointLight curr_light = lights->pointlights[curr];
    float curr_dist = distance(curr_light.position,cam_pos);
    while(distance(cam_pos,lights->pointlights[cmp].position)>curr_dist && cmp >= 0){
      --cmp;
    }
    PointLight cmp_light = lights->pointlights[cmp+1];
    lights->pointlights[cmp+1] = curr_light;
    lights->pointlights[curr] = cmp_light;
  }
}

LightSources* find_closest_lights(LightSources* lights, LightSources* nearest, int num_of_lights, vec3 cam_pos){
   sort_lights(lights, cam_pos);
   for(int n = 0; n < num_of_lights; ++n){
     nearest->pointlights[n] = lights->pointlights[n];
   }
   nearest->num_of_ptlights = num_of_lights;
   nearest->dirlight = *make_dir_light(SetVector(1.0,1.0,1.0),SetVector(0.1,0.1,0.1),SetVector(0.0,0.0,0.0),SetVector(0.0,0.0,0.0)); //Redish light
   return nearest;                                                   //change here
}
void apply_lighting(LightSources* lights, GLuint* shader, vec3 cam_pos){
  //Positional lights
  glUseProgram(*shader);
  glUniform3fv(glGetUniformLocation(*shader, "viewPos"),1,&cam_pos.x);
  LightSources* closest_lights = malloc(sizeof(LightSources)+12*sizeof(PointLight));;
  closest_lights->num_of_ptlights = 12;//Here we have specified 12 lights, could do more if we want to, however we would have to change the shader
  find_closest_lights(lights,closest_lights,closest_lights->num_of_ptlights,cam_pos);
  for (int i = 0; i < closest_lights->num_of_ptlights; i++)
  {
    char buffer[64];

    sprintf(buffer, "pointLights[%i].position", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &closest_lights->pointlights[i].position.x);

    sprintf(buffer, "pointLights[%i].constant", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), closest_lights->pointlights[i].constant);

    sprintf(buffer, "pointLights[%i].linear", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), closest_lights->pointlights[i].linear);

    sprintf(buffer, "pointLights[%i].quadratic", i);
    glUniform1f(glGetUniformLocation(*shader, buffer), closest_lights->pointlights[i].quadratic);

    sprintf(buffer, "pointLights[%i].ambient", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &closest_lights->pointlights[i].ambient.x);

    sprintf(buffer, "pointLights[%i].diffuse", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &closest_lights->pointlights[i].diffuse.x);

    sprintf(buffer, "pointLights[%i].specular", i);
    glUniform3fv(glGetUniformLocation(*shader, buffer), 1, &closest_lights->pointlights[i].specular.x);
  }
  //Directional lights
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.direction"), 1, &lights->dirlight.direction.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.ambient"), 1, &lights->dirlight.ambient.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.diffuse"), 1, &lights->dirlight.diffuse.x);
  glUniform3fv(glGetUniformLocation(*shader, "dirLight.specular"), 1, &lights->dirlight.specular.x);
}
