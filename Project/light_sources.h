#ifndef LIGHT_SOURCES_H
#define LIGHT_SOURCES_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"

//Struct for light_sources in a certain area
typedef struct PointLight PointLight;
typedef struct LightSources LightSources;

struct PointLight {
	  //Position of light
    vec3 position;
    //Attenuation of light
    float constant;
    float linear;
    float quadratic;
		//Lighting components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct LightSources{

	PointLight pointlights[4];
	DirLight dirlight;
  //GLuint id;
};

LightSources* lighting_heaven();
LightSources* lighting_hell();
//void apply_lighting(LightSources* Lights, GLuint* shader);

#endif //LIGHT_SOURCES_H
