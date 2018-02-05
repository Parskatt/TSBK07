// Lab 1-1.
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
GLfloat myTranslationMatrix[] =
{
	1.0f, 0.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

Model *m;
unsigned int bunnyVertexArrayObjID;

unsigned int bunnyVertexBufferObjID;

unsigned int bunnyIndexBufferObjID;

unsigned int bunnyNormalBufferObjID;
// Reference to program
GLuint program;

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.9,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-6.vert","lab1-6.frag");
	glUseProgram(program);
	printError("init shader");

	// Upload geometry to the GPU:
	m = LoadModel("bunny.obj");
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);
	glBindVertexArray(bunnyVertexArrayObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	//VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
	//VBO for index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
	// End of upload of geometry

	printError("init arrays");
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("pre display");
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "t"), t);
	SetRotationMatrix(t/1000.0, myRotMatrix);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	SetTranslationMatrix(t/1000.0, myTranslationMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatrix"), 1, GL_TRUE, myRotMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatrix2"), 1, GL_TRUE, myRotMatrix2);
	//glUniformMatrix4fv(glGetUniformLocation(program, "myTranslationMatrix"), 1, GL_TRUE, myTranslationMatrix);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
