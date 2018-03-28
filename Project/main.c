#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "cameramovement.h"
#include "load_shaders.h"
#include "objects.h"

//"Globals"
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir,cam_speed;
GLint prevx,prevy;
//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading;
//Make objects instead
WorldObject* octagon;
WorldObject* bunnyplus;
WorldObject* skybox;
void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	//Load Shaders
	load_shaders(&basic_shading,&skybox_shading, &advanced_shading);	// Load and compile shader
	//Camera init
	camera_init(&cam_pos,&cam_dir,&projectionMatrix,&worldToViewMatrix);
	//Make objects
	octagon = new_object("Textures/maskros512.tga", "Models/octagon.obj", T(0,0,10));
	bunnyplus = new_object("Textures/maskros512.tga", "Models/bunnyplus.obj", T(0,5,10));
	skybox = new_object("Textures/SkyBox512.tga", "Models/skybox.obj",T(cam_pos.x,cam_pos.y,cam_pos.z));
	glutPostRedisplay();
}

void timer(int i)
{
	glutTimerFunc(10, &timer, i);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the skybox
	glDisable(GL_DEPTH_TEST);
	skybox->position = T(cam_pos.x,cam_pos.y,cam_pos.z);
	render_object(skybox, &worldToViewMatrix, &projectionMatrix, &skybox_shading);
	glEnable(GL_DEPTH_TEST);
	//Draw other objects
	render_object(octagon, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_object(bunnyplus, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	glutSwapBuffers();
}

void mousefunc(int button, int state, int x, int y)
{
  mouse(button, state, x, y, &prevx, &prevy);
}

void mouseDraggedfunc(int x, int y)
{
  mouseDragged(x, y, &prevx, &prevy, &worldToViewMatrix, &cam_dir, &cam_pos);
}

void keyboardfunc(unsigned char c, int x, int y)
{
  keyboard(c, x, y, &worldToViewMatrix, &cam_pos, &cam_dir);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1920, 1080);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init();

  //TODO Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(10, &timer, 0);

	glutMainLoop();
	exit(0);
}
