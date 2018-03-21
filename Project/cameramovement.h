#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "MicroGlut.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

void mouse(int button, int state, int x, int y, int* prevx, int* prevy);
void mouseDragged(int x, int y, int* prevx, int* prevy, mat4* worldToViewMatrix, vec3* cam_dir, vec3* cam_pos);
void keyboard(unsigned char c, int x, int y, mat4* worldToViewMatrix, vec3* cam_pos, vec3* cam_dir);

#endif //CAMERAMOVEMENT_H