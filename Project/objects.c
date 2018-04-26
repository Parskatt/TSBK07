#include "objects.h"

WorldObject* new_object(GLuint tex_id, Model* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
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



ObjectList* create_static_objects(TextureList* textures, ModelList* models)
{
  ObjectList* out_list = malloc(sizeof(ObjectList)+10*sizeof(WorldObject));
  out_list->obj_list[0] = new_object(textures->texture_list[0], models->model_list[0], T(0,10,10));
  out_list->obj_list[1] = new_object(textures->texture_list[0], models->model_list[1], T(0,0,10));
  out_list->size = 2;
  return out_list;
}

ObjectList* create_torch_objects()
{
  ObjectList* out_list = malloc(sizeof(ObjectList)+sizeof(WorldObject)*500);
  out_list->size = 0;
  return out_list;
}

void add_torch(ObjectList* objects, TextureList* textures, ModelList* models, vec3 cam_pos)
{
  objects->obj_list[objects->size] = new_object(textures->texture_list[2], models->model_list[2], T(cam_pos.x,cam_pos.y-1,cam_pos.z));
  objects->obj_list[objects->size+1] = new_object(textures->texture_list[1], models->model_list[3], T(cam_pos.x,cam_pos.y-1,cam_pos.z));
  objects->size = objects->size + 2;
}

void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
  int i;
  //sizeof(a)/sizeof(a[0])
  //sizeof(objects->obj_list)/sizeof(objects->obj_list[0])
  //sizeof ger antal bytes, ex int-list ger 4 per byte, så delar på index0 (antal bytes per element)
  //TODO: Går det att använda detta för worldObj-pekare? Eller behövs en annan lista?
  //printf("Number of elements :%d", sizeof(objects->obj_list)/sizeof(objects->obj_list[0]));
  for (i=0; i<objects->size; i++)
  {
    render_object(objects->obj_list[i], worldToViewMatrix, projectionMatrix, shader);
  }
}


ModelList* load_models()
{
  ModelList* out_list = malloc(sizeof(ModelList));
  out_list->model_list[0] = LoadModelPlus("Models/bunnyplus.obj");
  out_list->model_list[1] = LoadModelPlus("Models/octagon.obj");
  out_list->model_list[2] = LoadModelPlus("Models/torch1.obj");
  out_list->model_list[3] = LoadModelPlus("Models/torch2.obj");

  out_list->size = 4;
  return out_list;
}

ModelList* load_torch_models()
{
  ModelList* out_list = malloc(sizeof(ModelList));
  out_list->size = 0;
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
