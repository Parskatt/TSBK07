#ifndef SKYBOX_H
#define SKYBOX_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct SkyBoxObject SkyBoxObject;

struct SkyBoxObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

SkyBoxObject* new_skybox(char* texture, Model* model, mat4 value);
void make_skybox_object(SkyBoxObject** skybox, char* texture1, char* texture2, char* texture3, char* texture4, char* texture5, char* texture6);
void render_skybox(SkyBoxObject** skybox, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
void draw_skybox(SkyBoxObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

#endif //OBJECTS_H
