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

typedef struct WorldObject WorldObject;

struct WorldObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

WorldObject* new_object(char* texture, char* model, mat4 value);
void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

#endif //OBJECTS_H