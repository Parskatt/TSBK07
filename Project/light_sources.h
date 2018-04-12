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
typedef struct DirLight DirLight;
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
struct DirLight {
	  //Direction of light
    vec3 direction;
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
PointLight* make_point_light(vec3 position,float constant,float linear,float quadratic,vec3 ambient,vec3 diffuse,vec3 specular);
DirLight* make_dir_light(vec3 direction,vec3 ambient,vec3 diffuse,vec3 specular);
LightSources* lighting_heaven();
LightSources* lighting_hell();
void apply_lighting(LightSources* lights, GLuint* shader);

#endif //LIGHT_SOURCES_H
