#include "objects.h"

WorldObject* new_object(char* texture, char* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  LoadTGATextureSimple(texture, &p->texture_id);
  p->model_ptr = LoadModelPlus(model);
  p->position = pos;
  return p;
}

void render_object(WorldObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader, GLint i){
  //Hopefully this will make rendercalls much less space wasting in the main loop :^)
  mat4 totMatrix;
  glUseProgram(*shader); //program used when drawing octagon

  if (i == 0)
  {
    worldToViewMatrix.m[3] = 0;
    worldToViewMatrix.m[7] = 0;
    worldToViewMatrix.m[11] = 0;
  }

	totMatrix = Mult(*projectionMatrix,Mult(worldToViewMatrix,object->position));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "totMatrix"), 1, GL_TRUE, totMatrix.m);
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}
