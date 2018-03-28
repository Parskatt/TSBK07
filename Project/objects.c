#include "objects.h"
//TODO add the specular component to the object maybe something else as well.
WorldObject* new_object(char* texture, char* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  LoadTGATextureSimple(texture, &p->texture_id);
  p->model_ptr = LoadModelPlus(model);
  p->position = pos;
  return p;
}

void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader){
  //Hopefully this will make rendercalls much less space wasting in the main loop :^)
  glUseProgram(*shader); //program used when drawing octagon
  glUniformMatrix4fv(glGetUniformLocation(*shader, "modelToWorldMatrix"), 1, GL_TRUE, object->position.m);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix->m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix->m);
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}
