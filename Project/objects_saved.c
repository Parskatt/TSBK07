#include "objects.h"

WorldObject* new_object(char* texture, char* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  LoadTGATextureSimple(texture, &p->texture_id);
  p->model_ptr = LoadModelPlus(model);
  p->position = pos;
  return p;
}

void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader){
  //Hopefully this will make rendercalls much less space wasting in the main loop :^)
  mat4 totMatrix;
  glUseProgram(*shader); //program used when drawing octagon
	totMatrix = Mult(*projectionMatrix,Mult(*worldToViewMatrix,object->position));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "totMatrix"), 1, GL_TRUE, totMatrix.m);
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}

void render_skybox(WorldObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader){
  //Hopefully this will make rendercalls much less space wasting in the main loop :^)
  mat4 totMatrix;
  glUseProgram(*shader); //program used when drawing octagon

  worldToViewMatrix.m[3] = 0;
  worldToViewMatrix.m[7] = 0;
  worldToViewMatrix.m[11] = 0;

	totMatrix = Mult(*projectionMatrix,Mult(worldToViewMatrix,object->position));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "totMatrix"), 1, GL_TRUE, totMatrix.m);
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}



ObjectList* create_objects()
{
  ObjectList* out_list = malloc(sizeof(ObjectList));
  out_list->obj_list[0] = new_object("Textures/girl-head.tga", "Models/bunnyplus.obj", T(0,10,10));
  //out_list->obj_list[1] = new_object("Textures/lava.tga", "Models/ground.obj", T(0,0,0));
  out_list->obj_list[1] = new_object("Textures/maskros512.tga", "Models/octagon.obj", T(0,0,10));
  //out_list->obj_list[3] = new_object("Textures/maskros512.tga", "Models/platform1.obj", T(0,0,0));
  return out_list;
}

void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
  int i;
  //sizeof(a)/sizeof(a[0])
  //sizeof(objects->obj_list)/sizeof(objects->obj_list[0])
  //sizeof ger antal bytes, ex int-list ger 4 per byte, så delar på index0 (antal bytes per element)
  //TODO: Går det att använda detta för worldObj-pekare? Eller behövs en annan lista?
  //printf("Number of elements :%d", sizeof(objects->obj_list)/sizeof(objects->obj_list[0]));
  for (i=0; i<2; i++)
  {
    render_object(objects->obj_list[i], worldToViewMatrix, projectionMatrix, shader);
  }
}


//h-file
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

//Define stuff for more advanced renderer
#define SKYBOX_SHADING 0
#define BASIC_SHADING 1
#define ADVANCED_SHADING 2

typedef struct WorldObject WorldObject;
typedef struct ObjectList ObjectList;


struct WorldObject{
  mat4 position;
  Model* model_ptr;
  GLuint texture_id;
};

struct ObjectList{
	int hej;
	WorldObject* obj_list[];
};

WorldObject* new_object(char* texture, char* model, mat4 value);
void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
void render_skybox(WorldObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);
ObjectList* create_objects();
void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader);

#endif //OBJECTS_H
