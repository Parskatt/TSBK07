#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

void load_shaders(GLuint *program1, GLuint *program2, GLuint *program3, GLuint *program4, GLuint *program5)
{
  *program1 = loadShaders("Shaders/basic.vert", "Shaders/basic.frag");
  *program2 = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
  *program3 = loadShaders("Shaders/advanced.vert", "Shaders/advanced.frag");
	*program4 = loadShaders("Shaders/splat.vert", "Shaders/splat.frag");
	*program5 = loadShaders("Shaders/particles.vert", "Shaders/particles.frag");
}
