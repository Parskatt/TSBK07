#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

void init_particles(int count, int height, GLuint* shader, unsigned int *vertexArrayObjID);
void render_rainy_cloud(int count, GLfloat a, GLuint* shader, unsigned int *vertexArrayObjID);

#endif //PARTICLESYSTEM_H
