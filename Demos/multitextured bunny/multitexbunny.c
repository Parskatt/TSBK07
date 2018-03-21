// Textured model with multitexture

// gcc multitexbunny.c ../common/*.c ../common/Linux/*.c -lGL -o multitexbunny -I../common -I../common/Linux -DGL_GLEXT_PROTOTYPES  -lXt -lX11 -lm

#include "MicroGlut.h"
// uses framework Cocoa
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

mat4 camMatrix, projectionMatrix;

// vertex array object
unsigned int bunnyVertexArrayObjID;
Model *m;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;

void init(void)
{
	// three vertex buffer objects, used for uploading the data
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glClearColor(1.0,1.0,1.0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("tex.vert", "tex.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	m = LoadModelPlus("bunnyplus.obj");

	// End of upload of geometry

	camMatrix = lookAt(0,0,-2, 0,0,0, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

	projectionMatrix = frustum(left, right, bottom, top, near, far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	printError("init arrays");
	
	glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1); // 5c
	LoadTGATextureSimple("Earth.tga", &tex2); // 5c

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
}

GLfloat a = 31.4/2;
GLfloat b = 0.0;
char anima = 0;
char animb = 0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (anima) a += 0.1;
	if (animb) b += 0.1;
	mat4 rot = Ry(a/10);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, rot.m);
	glUniform1f(glGetUniformLocation(program, "time"), b);

	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
	
	printError("display");
	
	glutSwapBuffers();
}

void key(unsigned char key, int x, int y)
{
	if (key == 'r')
		anima = !anima;
	if (key == 't' || key == ' ')
		animb = !animb;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("multitexbunny (r t)");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimerFunc(20);
	glutKeyboardFunc(key);
	glutMainLoop();
	exit(0);
}
