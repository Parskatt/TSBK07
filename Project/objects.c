#include objects.h

Object* Object_new(GLuint tex_id, char* model, mat4* value) {
  Object* p = malloc(sizeof(Object));
  p->texture_id = tex_id;
  p->model_ptr = *LoadModelPlus(*model);
  p->position = *value;
  return p;
}
