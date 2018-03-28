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
  ObjectList* out_struct = malloc(sizeof(ObjectList));
  out_struct->obj_list[0] = new_object("Textures/girl-head.tga", "Models/bunnyplus.obj", T(0,10,10));
  out_struct->obj_list[1] = new_object("Textures/lava.tga", "Models/ground.obj", T(0,0,0));
  out_struct->obj_list[2] = new_object("Textures/maskros512.tga", "Models/octagon.obj", T(0,0,10));
  return out_struct;
}

void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
  int i;
  for (i=0; i<3; i++)
  {
    render_object(objects->obj_list[i], worldToViewMatrix, projectionMatrix, shader);
  }
}
