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
  point3D positions[];
  point3D colors[];
  GLuint attenuation[];
  GLuint id;
};

LightSources* LightingHeaven();
LightSources* LightingHell();

#endif //LIGHT_SOURCES_H
