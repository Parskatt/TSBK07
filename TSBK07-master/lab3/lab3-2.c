// Lab 2-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"

//Defines
#define near 1.0
#define far 40.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


// Globals
// Data would normally be read from files

GLfloat myRotMatrix[] =
{
	1.0f, 0.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};
GLfloat myRotMatrix2[] =
{
	1.0f, 0.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};
GLfloat projectionMatrix[] =
{
  2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
  0.0f, 0.0f, -1.0f, 0.0f
};

mat4 rot, rot2, trans, total, worldToViewMatrix;

Model *blade1;
Model *blade2;
Model *blade3;
Model *blade4;
Model *walls;
Model *roof;
Model *balcony;

void LoadTGATextureSimple(char *filename, GLuint *tex);
// Reference to program
GLuint program;
GLuint myTex;
GLuint myTex2;

// vertex array object
unsigned int vertexArrayObjID;
unsigned int vertexArrayObjID2;
void init(void)
{
	dumpInfo();
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	// GL inits
	glClearColor(0.9,0.8,0.5,0);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert","lab3-1.frag");
	glUseProgram(program);
	printError("init shader");

	// Upload geometry to the GPU:
	blade1 = LoadModelPlus("windmill/blade.obj");
	blade2 = LoadModelPlus("windmill/blade.obj");
	blade3 = LoadModelPlus("windmill/blade.obj");
	blade4 = LoadModelPlus("windmill/blade.obj");
  walls = LoadModelPlus("windmill/windmill-walls.obj");
	balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	roof = LoadModelPlus("windmill/windmill-roof.obj");
	//glActiveTexture(GL_TEXTURE0);

  //Frustum matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init arrays");
	// End of upload of geometry

}
void SetRotationMatrix(GLfloat t, GLfloat *m)
{
m[0] = cos(t); m[1] = 0; m[2] = -sin(t); m[3] = 0.0;
m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0.0;
m[8] = sin(t); m[9] = 0; m[10] = cos(t); m[11] = 0.0;
m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
}
void SetRotationMatrix2(GLfloat t, GLfloat *m)
{
m[0] = 1; m[1] = 0; m[2] =0; m[3] = 0.0;
m[4] = 0; m[5] = cos(t); m[6] =  -sin(t); m[7] = 0.0;
m[8] = 0; m[9] = sin(t); m[10] = cos(t); m[11] = 0.0;
m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
}

void SetTranslationMatrix(GLfloat t, GLfloat *m)
{
m[0] = 1.0; m[1] = 0.0; m[2] = 0; m[3] = sin(t);
m[4] = 0.0; m[5] = 1.0; m[6] = 0; m[7] = cos(2*t);
m[8] = 0; m[9] = 0; m[10] = 1.0; m[11] = 0;
m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	printError("pre display");
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
  worldToViewMatrix = lookAt(25*sin(t/1000.0), 0, 25*cos(t/1000.0), 0,0,0, 0,1,0);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

  //For Model 1
  trans = T(5, 4.2, 0);
  rot = Ry(3.14);
	rot2 = Rx(0+t/100.0);
  total = Mult(trans,Mult(rot,rot2));
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade1, program, "in_Position", "inNormal", "inTexCoord");


  rot = Ry(3.14);
	rot2 = Rx(3.14*0.5+t/100.0);
  total = Mult(Mult(trans,rot), rot2);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade2, program, "in_Position", "inNormal", "inTexCoord");


  rot = Ry(3.14);
	rot2 = Rx(3.14+t/100.0);
  total = Mult(trans,Mult(rot, rot2));
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade3, program, "in_Position", "inNormal", "inTexCoord");


	rot = Ry(3.14);
	rot2 = Rx(3.14*1.5+t/100.0);
	total = Mult(Mult(trans,rot), rot2);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade4, program, "in_Position", "inNormal", "inTexCoord");

  //For Model 2
  trans = T(0, -5, 0);
  rot = Ry(0);
  total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(walls, program, "in_Position", "inNormal", "inTexCoord");

	// For model 3
	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof, program, "in_Position", "inNormal", "inTexCoord");

	// For model 4
	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(balcony, program, "in_Position", "inNormal", "inTexCoord");

	printError("display");
	glutSwapBuffers();
}

GLfloat a, b = 0.0;

void mouse(int x, int y)
{
	b = x * 1.0;
	a = y * 1.0;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("GL3 triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	init();
	glutMainLoop();
	glutPassiveMotionFunc(mouse);
	return 0;
}
