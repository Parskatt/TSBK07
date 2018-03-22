#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

void load_shaders(int *program1, int *program2, int *program3)
{
  *program1 = loadShaders("Shaders/basic.vert", "Shaders/basic.frag");
  *program2 = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
  *program3 = loadShaders("Shaders/advanced.vert", "Shaders/advanced.frag");
}
