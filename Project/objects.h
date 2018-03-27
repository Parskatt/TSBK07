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

struct Object{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
}

Object* Object_new(int tex_id, char* model, mat4* value);

#endif //OBJECTS_H
