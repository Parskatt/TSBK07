#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "cameramovement.h"

//Globals
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir;
GLint prevx,prevy;

Model *octa;
GLuint program;
//GLuint maskros_tex;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
	program = loadShaders("Shaders/basic.vert", "Shaders/basic.frag");	// Load and compile shader
	glUseProgram(program);
	printError("init shader");

	cam_pos = SetVector(0, 0, 0);
	cam_dir = SetVector(0, 0, 20);
	worldToViewMatrix = lookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	modelToWorldMatrix = T(0,0,10);

	//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	//glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	// Load models
	octa = LoadModelPlus("Models/octagon.obj");
	//glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	//LoadTGATextureSimple("Textures/maskros512.tga", &maskros_tex);
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
	glUseProgram(program); //program used when drawing octagon
	totMatrix = Mult(projectionMatrix,Mult(worldToViewMatrix,modelToWorldMatrix));
	//modelworld = IdentityMatrix();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, maskros_tex);
	glUniformMatrix4fv(glGetUniformLocation(program, "totMatrix"), 1, GL_TRUE, totMatrix.m);
	DrawModel(octa, program, "inPosition", "inNormal", "inTexCoord");
	printf("cam_pos: %1f %1f %1f \n", cam_pos.x, cam_pos.y, cam_pos.z);
	printf("cam_dir: %1f %1f %1f \n", cam_dir.x, cam_dir.y, cam_dir.z);
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

  //Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
