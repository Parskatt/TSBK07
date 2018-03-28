#include "light_sources.h"



//This is ugly but it has to be done somewhere. (If you're really ambitious you could place this in a file and read from file)
LightSources* lighting_hell(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {1.0f, 0.0f, 0.0f}, // Red light, positional
                              {1.0f, 1.0f, 0.0f}};
  lights->attenuation = {0.5,0.01};
  return lights
}

LightSources* lighting_heaven(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {0.0f, 0.0f, 1.0f}, // Blue light, positional
                              {1.0f, 1.0f, 1.0f}}; //White light
  lights->attenuation = {0.5,0.01};
  return lights
};
void apply_lighting(LightSources* Lights, GLuint* shader){
  Point3D LightPos = Lights->positions;
  Point3D LightColor = Lights->colors;
  glUniform3fv(glGetUniformLocation(shader, "LightPos"), 2, &LightPos[0].x);
	glUniform3fv(glGetUniformLocation(shader, "LightColor"), 2, &LightColor[0].x);
}
