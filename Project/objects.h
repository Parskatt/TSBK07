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

typedef struct ModelList ModelList;
typedef struct TextureList TextureList;
typedef struct WorldObject WorldObject;
typedef struct ObjectList ObjectList;

struct ModelList{
	int size;
	Model* model_list[];
};

struct TextureList{
	int size;
	GLuint texture_list[];
};

struct WorldObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

struct ObjectList{
	int hej;
	WorldObject* obj_list[];
};

WorldObject* new_object(GLuint tex_id, Model* model, mat4 value);
//WorldObject* new_skybox(char* texture, char* model, mat4 pos);
void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
//void render_skybox(WorldObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
ObjectList* create_objects(TextureList* textures, ModelList* models);
void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
ModelList* load_models();
TextureList* load_textures();

#endif //OBJECTS_H
