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
typedef struct SplatTerrain SplatTerrain;

struct TerrainObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

struct SplatTerrain{
	mat4 position;
	Model* model_ptr;
	GLuint texture_id[4];
};

TerrainObject* new_terrain(char* texture, char* map, mat4 pos, int height_scale);
SplatTerrain* new_splat(char* tex1, char* tex2, char* tex3, char* heightmap, char*splatmap, mat4 pos, int height_scale);
void render_splat(SplatTerrain* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
void render_terrain(TerrainObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

Model* GenerateTerrain(TextureData *tex, int height_scale);




#endif //TERRAIN_H
