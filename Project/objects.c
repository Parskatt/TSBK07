#include "objects.h"
#define pi 3.1415926
WorldObject* new_object(GLuint tex_id, Model* model, mat4 pos) {
  WorldObject* p = malloc(sizeof(WorldObject));
  p->texture_id = tex_id;
  p->model_ptr = model;
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
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object->texture_id);
  //Draw the model
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}

GLfloat vertices_p[] = {
 -0.1f, -0.1f, 0.0f,
 0.1f, -0.1f, 0.0f,
 -0.1f, 0.1f, 0.0f
};

 GLubyte minitex[5][4][3] =
 {
	 { {100, 20, 49}, { 85, 6,29}, { 80, 5,53}, { 72,9,64}},
	 { { 95, 34,78}, {98, 248,43}, { 101,32,78}, { 77, 87,90}},
	 { { 80, 65,44}, { 59,255,67}, {76,50,70}, { 58, 50,54}},
	 { { 78,255,33}, { 91, 9,60}, { 108, 66,52}, {105, 50,44}},
   { { 49,205,37}, { 87, 29,68}, { 78, 69,42}, {105, 60,46}},
 };

  GLuint minitexid;

void render_particles(mat4* pos, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader, unsigned int *vertexArrayObjID, int num_particles, int width, int height, GLfloat t){
  glUseProgram(*shader);
  //Send matrices to shader
  glUniformMatrix4fv(glGetUniformLocation(*shader, "modelToWorldMatrix"), 1, GL_TRUE, pos->m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix->m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix->m);
  glUniform1i(glGetUniformLocation(*shader, "tex"), 0); // Texture unit 0
  glUniform1f(glGetUniformLocation(*shader, "time"), t);
  int speed = 1; //use if time to implement acceleration
  glUniform1f(glGetUniformLocation(*shader, "speed"), speed);
  glBindVertexArray(*vertexArrayObjID);	// Select VAO
  glBindTexture(GL_TEXTURE_2D, minitexid);
  glActiveTexture(GL_TEXTURE0);
  glDrawArraysInstanced(GL_POINTS, 0, 4, num_particles);
}

void init_particles(GLuint* shader, unsigned int *vertexArrayObjID, int num_particles, int width, int height) {

  unsigned int vertexBufferObjID;
  glUseProgram(*shader);
  glGenVertexArrays(1, &(*vertexArrayObjID));
  glBindVertexArray(*vertexArrayObjID);
  glGenBuffers(1, &vertexBufferObjID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_p), vertices_p, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(*shader, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(*shader, "in_Position"));
  //upload to shader
  glUniform1i(glGetUniformLocation(*shader, "tex"), 0);
  glUniform1f(glGetUniformLocation(*shader, "num_particles"), num_particles);
  glUniform1f(glGetUniformLocation(*shader, "width"), width);
  glUniform1f(glGetUniformLocation(*shader, "height"), height);

  //minitex init
  glGenTextures(1, &minitexid);
  glBindTexture(GL_TEXTURE_2D, minitexid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, minitex);
  glActiveTexture(GL_TEXTURE0);
}


ObjectList* create_static_objects(TextureList* textures, ModelList* models)
{
  ObjectList* out_list = malloc(sizeof(ObjectList)+10*sizeof(WorldObject));
  out_list->obj_list[0] = new_object(textures->texture_list[0], models->model_list[0], T(0,10,10));
  out_list->obj_list[1] = new_object(textures->texture_list[0], models->model_list[1], T(0,0,10));
  out_list->obj_list[2] = new_object(textures->texture_list[3], models->model_list[4], Mult(T(135,100,133),S(0.03, 0.03, 0.03)));
  out_list->obj_list[3] = new_object(textures->texture_list[3], models->model_list[5], T(222,0,228));//ArbRigid(SetVector(0,1,0),pi/6,ring_pos));
  out_list->size = 4;
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

void render_objects(ObjectList* objects, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader, int remove_object)
{
  for(int i=0; i<(objects->size-remove_object); i++)//remove_object insert here
  {
    render_object(objects->obj_list[i], worldToViewMatrix, projectionMatrix, shader);
  }

}


ModelList* load_models()
{
  ModelList* out_list = malloc(10*sizeof(ModelList));
  out_list->model_list[0] = LoadModelPlus("Models/bunnyplus.obj");
  out_list->model_list[1] = LoadModelPlus("Models/octagon.obj");
  out_list->model_list[2] = LoadModelPlus("Models/torch1.obj");
  out_list->model_list[3] = LoadModelPlus("Models/torch2.obj");
  out_list->model_list[4] = LoadModelPlus("Models/cloud.obj");
  out_list->model_list[5] = LoadModelPlus("Models/torus.obj");


  out_list->size = 6;
  return out_list;
}

ModelList* load_torch_models()
{
  ModelList* out_list = malloc(100*sizeof(ModelList));
  out_list->size = 0;
  return out_list;
}


TextureList* load_textures()
{
  TextureList* out_list = malloc(10*sizeof(TextureList));
  LoadTGATextureSimple("Textures/maskros512.tga", &out_list->texture_list[0]);
  LoadTGATextureSimple("Textures/lava.tga", &out_list->texture_list[1]);
  LoadTGATextureSimple("Textures/kt_rock_1f_dk.tga", &out_list->texture_list[2]);
  LoadTGATextureSimple("Textures/moln.tga", &out_list->texture_list[3]);

  out_list->size = 4;
  return out_list;
}
