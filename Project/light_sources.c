#include "light_sources.h"



//This is ugly but it has to be done somewhere. (If you're really ambitious you could place this in a file and read from file)
LightSources* LightingHell(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {1.0f, 0.0f, 0.0f}, // Red light, positional
                              {1.0f, 1.0f, 0.0f}};
  lights->attenuation = {0.5,0.01};
  return lights
}

LightSources* LightingHeaven(){
  LightSources* lights;
  lights->positions = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                              {0.0f, 5.0f, 10.0f}}; // Yellow light, positional
  lights->colors = { {0.0f, 0.0f, 1.0f}, // Blue light, positional
                              {1.0f, 1.0f, 1.0f}}; //White light
  lights->attenuation = {0.5,0.01};
  return lights
};
