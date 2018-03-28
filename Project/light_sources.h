#ifndef LIGHT_SOURCES_H
#define LIGHT_SOURCES_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"

//Struct for light_sources in a certain area

typedef struct LightSources LightSources;

struct LightSources{
  Point3D positions[2];
  Point3D colors[2];
  GLuint attenuation[2];
  //GLuint id;
};

LightSources* lighting_heaven();
LightSources* lighting_hell();
void apply_lighting(LightSources* Lights, GLuint* shader);

#endif //LIGHT_SOURCES_H
