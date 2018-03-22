#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "cameramovement.h"

mat4 projectionMatrix,worldToViewMatrix;
vec3 cam_pos,cam_dir;
GLint prevx,prevy;
GLuint program;
Model *octa;

void init(void)
{
	// GL inits
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
	program = loadShaders("Shaders/basic.vert", "Shaders/basic.frag");	// Load and compile shader
	glUseProgram(program);
	printError("init shader");

	cam_pos = SetVector(0, 0, 30);
	cam_dir = SetVector(0, 0, 20);

	worldToViewMatrix = lookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);

  // Load models
	octa = LoadModelPlus("Models/octagon.obj");
	//glutPostRedisplay();
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 total = IdentityMatrix();

	//octagon model
	glUniformMatrix4fv(glGetUniformLocation(program, "totMatrix"), 1, GL_TRUE, total.m);
	DrawModel(octa, program, "inPosition", "inNormal", "inTexCoord");
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init();

  //Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
