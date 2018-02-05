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
#define far 30.0
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

mat4 rot, trans, total, worldToViewMatrix;

Model *m;
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;
Model *m2;
unsigned int bilVertexArrayObjID;
unsigned int bilVertexBufferObjID;
unsigned int bilIndexBufferObjID;
unsigned int bilNormalBufferObjID;
unsigned int bilTexCoordBufferObjID;
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
	program = loadShaders("lab2-7.vert","lab2-7.frag");
	glUseProgram(program);
	printError("init shader");

	// Upload geometry to the GPU:
	m = LoadModelPlus("bunnyplus.obj");
  m2 = LoadModelPlus("bilskiss.obj");

  //Frustum matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	// load texture
  LoadTGATextureSimple("maskros512.tga", &myTex);
	LoadTGATextureSimple("bilskissred.tga", &myTex2);
  glActiveTexture(GL_TEXTURE0);   // activate texture
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
  worldToViewMatrix = lookAt(10*sin(t/1000), 5, -10*cos(t/1000), 0,0,0, 0,1,0);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

  //For Model 1
  glBindTexture(GL_TEXTURE_2D, myTex);
  trans = T(0, 0, -2);
  rot = Ry(0);//t/1000.0);
  total = Mult(trans,rot);
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	//glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO

	DrawModel(m, program, "in_Position", "inNormal", "inTexCoord");

  glBindTexture(GL_TEXTURE_2D, myTex2);
  //For Model 2
  trans = T(0, -2, -5);
  rot = Ry(0);
  total = Mult(trans,rot);
	//glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	//dont clear the screen
	//glBindVertexArray(bilVertexArrayObjID);    // Select VAO
  //glActiveTexture(GL_TEXTURE1);   // activate texture

	DrawModel(m2, program, "in_Position", "inNormal", "inTexCoord");



	printError("display");
	glutSwapBuffers();
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
	return 0;
}
