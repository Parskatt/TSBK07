// Phong shaded bunny
// Note: Simplified! In particular, the light source is given in view
// coordinates, which means that it will follow the camera.
// You usually give light sources in world coordinates.

// 2017: Since the main program wasn't interestning, and the shader exists in other demos, I also added the feature to control rotation with mouse buttons. Right and left steps rotation, and the middle button activates continuous animation until yuo release the button.

// gcc phongbunny.c ../common/*.c ../common/Linux/*.c -lGL -o phongbunny -I../common -I../common/Linux -DGL_GLEXT_PROTOTYPES  -lXt -lX11 -lm

#include "MicroGlut.h"
//uses framework Cocoa
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"

mat4 projectionMatrix;

// Pointer to model data
Model *bunny;

// Reference to shader program
GLuint program;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 300.0);

	// Load and compile shader
	program = loadShaders("phong.vert", "phong.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("bunny.obj");

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	printError("init arrays");
}

GLfloat a = 0.0;
char animate = 0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Matrix4D worldToView, m1, m2, m, tr, scale;
	worldToView = lookAt(0,0,2, 0,0,0, 0,1,0);
	
	if (animate)
		a += 0.1;
	m1 = Rz(M_PI/5);
	m2 = Ry(a);
	m = Mult(m2,m1);
	m = Mult(worldToView, m);
	
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(bunny, program, "inPosition", "inNormal", "");
	
	printError("display");
	
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
}

void mouse(int button, int state, int x, int y)
{
// Note that state should be inspected if we only want to react on mouse down. Check for GLUT_UP and GLUT_DOWN.
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			a += 0.3;break;
		case GLUT_RIGHT_BUTTON:
			a -= 0.3;break;
		case GLUT_MIDDLE_BUTTON:
			animate = (state == GLUT_DOWN);break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 phong bunny example");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	init ();
	glutMainLoop();
}
