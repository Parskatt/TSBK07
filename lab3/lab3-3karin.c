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
#define far 1000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
//#define TEXTURE_OFFSET 0 //Select texture set by setting this constant to 0

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
GLfloat a, b, prevx, prevy = 0.0;
vec3 cam_pos;
mat4 rot, rot2, trans, total, worldToViewMatrix;

Model *blade1;
Model *blade2;
Model *blade3;
Model *blade4;
Model *walls;
Model *roof;
Model *balcony;
Model *ground;
Model *skybox;

void LoadTGATextureSimple(char *filename, GLuint *tex);
// Reference to program
GLuint program;
GLuint skynet_program;
GLuint rutorTex;
GLuint grassTex;
GLuint dirtTex;
GLuint skyTex;

void init(void)
{
	dumpInfo();
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	// GL inits
	glClearColor(0.9,0.8,0.5,0);
	printError("GL inits");
	cam_pos = SetVector(0.0, 0.0, 25.0);
	// Load and compile shaders
	program = loadShaders("lab3-3karin.vert","lab3-3karin.frag");
	skynet_program = loadShaders("lab3-3skynetkarin.vert","lab3-3skynetkarin.frag");

	printError("init shader");
	worldToViewMatrix = lookAt(0, 0, 25, 0,0,0, 0,1,0);

	// Upload geometry to the GPU:
	//Windmill
	blade1 = LoadModelPlus("windmill/blade.obj");
	blade2 = LoadModelPlus("windmill/blade.obj");
	blade3 = LoadModelPlus("windmill/blade.obj");
	blade4 = LoadModelPlus("windmill/blade.obj");
  walls = LoadModelPlus("windmill/windmill-walls.obj");
	balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	roof = LoadModelPlus("windmill/windmill-roof.obj");
	//Ground
	ground = LoadModelPlus("ground.obj");

	//Load textures
	glActiveTexture(GL_TEXTURE0);   // activate texture
	LoadTGATextureSimple("rutor.tga", &rutorTex);
	LoadTGATextureSimple("grass.tga", &grassTex);
	LoadTGATextureSimple("dirt.tga", &dirtTex);

  //Frustum matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init arrays");

	//Skybox
	skybox = LoadModelPlus("skybox.obj");
	LoadTGATextureSimple("SkyBox512.tga", &skyTex);
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
  //glUseProgram(skynet_program);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

/*
	//Skybox
	glUseProgram(skynet_program);
	glDisable(GL_DEPTH_TEST);
	//glUniformMatrix4fv(glGetUniformLocation(skynet_program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	DrawModel(skybox, skynet_program, "in_Position", "inNormal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	//End skybox


  //glUniformMatrix4fv(glGetUniformLocation(skynet_program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, skyTex);
	trans = T(5, 4.2, 0);
  rot = Ry(3.14);
	rot2 = Rx(0+t/500.0);
  total = Mult(trans,Mult(rot,rot2));
	glUniform1f(glGetUniformLocation(skynet_program, "t"), t);
	glUniform1i(glGetUniformLocation(skynet_program, "texUnit"), 0);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(skynet_program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(skybox, skynet_program, "in_Position", "inNormal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);*/
	glUseProgram(skynet_program);
	//glUniformMatrix4fv(glGetUniformLocation(skynet_program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	printError("display");
	glUseProgram(program);
	printError("pre display");
  //worldToViewMatrix = lookAt(25*sin(t/1000.0), 0, 25*cos(t/1000.0), 0,0,0, 0,1,0);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

  //The Windmill Model
	//Blades
	glBindTexture(GL_TEXTURE_2D, dirtTex);

  trans = T(5, 4.2, 0);
  rot = Ry(3.14);
	rot2 = Rx(0+t/500.0);
  total = Mult(trans,Mult(rot,rot2));
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade1, program, "in_Position", "inNormal", "inTexCoord");

	rot2 = Rx(3.14*0.5+t/500.0);
  total = Mult(Mult(trans,rot), rot2);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade2, program, "in_Position", "inNormal", "inTexCoord");

	rot2 = Rx(3.14+t/500.0);
  total = Mult(trans,Mult(rot, rot2));
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade3, program, "in_Position", "inNormal", "inTexCoord");

	rot2 = Rx(3.14*1.5+t/500.0);
	total = Mult(Mult(trans,rot), rot2);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade4, program, "in_Position", "inNormal", "inTexCoord");

  //Walls
  trans = T(0, -5, 0);
  rot = Ry(0);
  total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(walls, program, "in_Position", "inNormal", "inTexCoord");

	//Roof
	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof, program, "in_Position", "inNormal", "inTexCoord");

	//Balcony
	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	SetRotationMatrix2(t/1000.0, myRotMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(balcony, program, "in_Position", "inNormal", "inTexCoord");

	//The Ground Model
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(ground, program, "in_Position", "inNormal", "inTexCoord");

	printError("display");
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
	}
}

void mouseDragged(int x, int y)
{
	vec3 p;
	mat4 m;

	p.y = prevx-x;
	p.x = -(y-prevy);
	p.z = 0;
	mat3 wv3 = mat4tomat3(worldToViewMatrix);
	p = MultMat3Vec3(InvertMat3(wv3), p);
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 50.0);
	cam_pos = MultMat3Vec3(mat4tomat3(m),cam_pos);
	worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, 0,0,0, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	//glUniformMatrix4fv(glGetUniformLocation(skynet_program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	prevx = x;
	prevy = y;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("GL3 triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	glutPassiveMotionFunc(mouse);
	glutMotionFunc(mouseDragged);
	init();
	glutMainLoop();
	return 0;
}
