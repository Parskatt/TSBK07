#ifndef OBJECTS_H
#define OBJECTS_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "light_sources.h"

//Define stuff for more advanced renderer maybe lul
#define SKYBOX_SHADING 0
#define BASIC_SHADING 1
#define ADVANCED_SHADING 2

typedef struct WorldObject WorldObject;

struct WorldObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

WorldObject* new_object(char* texture, char* model, mat4 value);
void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

#endif //OBJECTS_H
