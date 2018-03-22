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

//Globals
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir;
GLint prevx,prevy;
//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading;
//Initialize Model stuff
Model *octa;
//GLuint maskros_tex;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
	//Load Shaders
	load_shaders(&basic_shading);	// Load and compile shader
	glUseProgram(basic_shading);
	printError("init shader");
	cam_dir = SetVector(0, 0, 20);
	worldToViewMatrix = lookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	modelToWorldMatrix = T(0,0,10);
	// Load models
	octa = LoadModelPlus("Models/octagon.obj");
  printError("init");
	glutPostRedisplay();
}

void timer(int i)
{
	glutPostRedisplay();
	glutTimerFunc(20, &timer, i);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(basic_shading); //program used when drawing octagon
	totMatrix = Mult(projectionMatrix,Mult(worldToViewMatrix,modelToWorldMatrix));
	glUniformMatrix4fv(glGetUniformLocation(basic_shading, "totMatrix"), 1, GL_TRUE, totMatrix.m);
	DrawModel(octa, basic_shading, "inPosition", NULL, "inTexCoord");
	printError("display");
	glutSwapBuffers();
}

void mousefunc(int button, int state, int x, int y)
{
  mouse(button, state, x, y, &prevx, &prevy);
	glutPostRedisplay();
}

void mouseDraggedfunc(int x, int y)
{
  mouseDragged(x, y, &prevx, &prevy, &worldToViewMatrix, &cam_dir, &cam_pos);
	glutPostRedisplay();
}

void keyboardfunc(unsigned char c, int x, int y)
{
  keyboard(c, x, y, &worldToViewMatrix, &cam_pos, &cam_dir);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init();

  //TODO Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
