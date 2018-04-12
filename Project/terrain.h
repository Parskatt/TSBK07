#ifndef TERRAIN_H
#define TERRAIN_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct TerrainObject TerrainObject;

struct TerrainObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

TerrainObject* new_terrain(char* texture, mat4 pos, int height_scale);
void render_terrain(TerrainObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

Model* GenerateTerrain(TextureData *tex, int height_scale);




#endif //TERRAIN_H
