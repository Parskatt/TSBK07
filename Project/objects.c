#include "objects.h"
<<<<<<< HEAD



WorldObject* new_object(GLuint tex_id, Model* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  //LoadTGATextureSimple(texture, &p->texture_id);
  //p->model_ptr = LoadModelPlus(model);
  p->texture_id = tex_id;
  p->model_ptr = model;
  p->position = pos;
  return p;
}

WorldObject* new_skybox(char* texture, char* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  LoadTGATextureSimple(texture, &p->texture_id);
  p->model_ptr = LoadModelPlus(model);
  p->position = pos;
  return p;
}
void render_object(WorldObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader){
  //Hopefully this will make rendercalls much less space wasting in the main loop :^)
  glUseProgram(*shader); //program used when drawing octagon
  //Send matrices to shader
  glUniformMatrix4fv(glGetUniformLocation(*shader, "modelToWorldMatrix"), 1, GL_TRUE, object->position.m);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix->m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix->m);
  //Send textures to shader
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
  //Draw the model
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



ObjectList* create_objects(TextureList* textures, ModelList* models)
{
  ObjectList* out_list = malloc(sizeof(ObjectList));
  out_list->obj_list[0] = new_object(textures->texture_list[0], models->model_list[0], T(0,10,10));
  //out_list->obj_list[1] = new_object("Textures/lava.tga", "Models/ground.obj", T(0,0,0));
  out_list->obj_list[1] = new_object(textures->texture_list[0], models->model_list[1], T(0,0,10));
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


ModelList* load_models()
{
  ModelList* out_list = malloc(sizeof(ModelList));
  out_list->model_list[0] = LoadModelPlus("Models/bunnyplus.obj");
  out_list->model_list[1] = LoadModelPlus("Models/octagon.obj");

  out_list->size = 2;
  return out_list;
}

TextureList* load_textures()
{
  TextureList* out_list = malloc(sizeof(TextureList));
  LoadTGATextureSimple("Textures/maskros512.tga", &out_list->texture_list[0]);
  LoadTGATextureSimple("Textures/lava.tga", &out_list->texture_list[1]);
  LoadTGATextureSimple("Textures/kt_rock_1f_dk.tga", &out_list->texture_list[2]);

  out_list->size = 3;
  return out_list;
}
