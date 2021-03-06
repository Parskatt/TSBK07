// Lab 2-3.
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
#include "LoadTGA.h"
#include <stdio.h>
#include <string.h>

//Defines
#define near 1.0
#define far 200.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


// Globals
// Data would normally be read from files
GLfloat a, b, prevx, prevy = 0.0;
vec3 cam_pos, cam_dir, cam_up, v;
mat4 rot, rot2, trans, scale, total, worldToViewMatrix;
GLuint program, program_skynet;
GLuint groundTex, skyTex, maskrosTex, dirtTex;

Model *blade1, *blade2, *blade3, *blade4, *walls, *roof, *balcony, *ground, *skybox, *bunny;

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

Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

                                 {0.0f, 1.0f, 0.0f}, // Green light

                                 {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f} }; // White light

Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

	                                        {0.0f, 5.0f, 10.0f}, // Green light, positional

	                                        {-1.0f, 0.0f, 0.0f}, // Blue light along X

	                                        {0.0f, 0.0f, -1.0f} }; // White light along Z


const GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};

GLint isDirectional[] = {0,0,1,1};

#define TEXTURE_OFFSET 0 //Select texture set by setting this constant to 0

void LoadTGATextureSimple(char *filename, GLuint *tex);


void init(void)
{
	dumpInfo();
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glEnable(GL_DEPTH_TEST);

	// GL inits
	glClearColor(0.9,0.8,0.5,0); //makes the yellow ground, "sand"
	printError("GL inits");
	cam_pos = SetVector(0.0, 0.0, 25.0);
	cam_dir = SetVector(0.0, 0.0, 0.0);
	//cam_up = SetVector(0.0,1.0,0.0);
	v = SetVector(0.0, 0.0, 0.0);
	// Load and compile shader

	program_skynet = loadShaders("lab3-4skybox.vert","lab3-4skybox.frag");
	glUseProgram(program_skynet);
	skybox = LoadModelPlus("models/skybox.obj");
	LoadTGATextureSimple("SkyBox512.tga", &skyTex);
	glUniformMatrix4fv(glGetUniformLocation(program_skynet, "projMatrix"), 1, GL_TRUE, projectionMatrix); //edit: different projmatrices
	glUniform1i(glGetUniformLocation(program_skynet, "skyTex"), 0);
	printError("init shader");

	program = loadShaders("lab3-4.vert","lab3-4.frag");
	worldToViewMatrix = lookAt(0, 0, 25, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);

	// Upload geometry to the GPU:
	blade1 = LoadModelPlus("windmill/blade.obj");
	blade2 = LoadModelPlus("windmill/blade.obj");
	blade3 = LoadModelPlus("windmill/blade.obj");
	blade4 = LoadModelPlus("windmill/blade.obj");
  walls = LoadModelPlus("windmill/windmill-walls.obj");
	balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	roof = LoadModelPlus("windmill/windmill-roof.obj");
	ground = LoadModelPlus("models/ground.obj");
	bunny = LoadModelPlus("models/bunnyplus.obj");


	// Load textures
	LoadTGATextureSimple("grass.tga", &groundTex);
	LoadTGATextureSimple("maskros512.tga", &maskrosTex);
	LoadTGATextureSimple("dirt.tga", &dirtTex);

  //Frustum matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	//glUniform1i(glGetUniformLocation(program, "groundTex"), 0);

	printError("init arrays");
	// End of upload of geometry

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

	//Make the time uniform
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	//Skybox Rendering
	glDisable(GL_DEPTH_TEST);
	glUseProgram(program_skynet);

	//worldToViewMatrix
	glUniformMatrix4fv(glGetUniformLocation(program_skynet, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyTex);

	trans = T(cam_pos.x,cam_pos.y,cam_pos.z);
	rot = Ry(0);
	total = Mult(trans,rot);
	glUniformMatrix4fv(glGetUniformLocation(program_skynet, "mdlMatrix"), 1, GL_TRUE, total.m);


	DrawModel(skybox, program_skynet, "in_Position", NULL, "inTexCoord");

	printError("sad");

	// Ground model
	glUniform3f(glGetUniformLocation(program, "myPos"), cam_pos.x, cam_pos.y, cam_pos.z);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &specularExponent[0]);

	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTex);

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

	trans = T(0,-5,0);
	rot = Ry(0);
	total = Mult(trans,rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	DrawModel(ground, program, "in_Position", "inNormal", "inTexCoord");
	printError("asd");


  //Windmill rendering
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &specularExponent[1]);
  trans = T(5, 4.2, 0);
  rot = Ry(3.14);
	rot2 = Rx(0+t/500.0);
  total = Mult(trans,Mult(rot,rot2));
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade1, program, "in_Position", "inNormal", "inTexCoord");
	printError("Hej");
	rot2 = Rx(3.14*0.5+t/500.0);
  total = Mult(Mult(trans,rot), rot2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade2, program, "in_Position", "inNormal", "inTexCoord");

	rot2 = Rx(3.14+t/500.0);
  total = Mult(trans,Mult(rot, rot2));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade3, program, "in_Position", "inNormal", "inTexCoord");

	rot2 = Rx(3.14*1.5+t/500.0);
	total = Mult(Mult(trans,rot), rot2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade4, program, "in_Position", "inNormal", "inTexCoord");

  trans = T(0, -5, 0);
  rot = Ry(0);
  total = Mult(trans,rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(walls, program, "in_Position", "inNormal", "inTexCoord");

	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof, program, "in_Position", "inNormal", "inTexCoord");

	trans = T(0, -5, 0);
	rot = Ry(0);
	total = Mult(trans,rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(balcony, program, "in_Position", "inNormal", "inTexCoord");
	//end of windmill

	//Bunny rendering
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &specularExponent[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, maskrosTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);


	trans = T(-10,10,0);
	rot = Ry(0);
	scale = S(10, 10, 10);
	total = Mult(trans, rot);
	total = Mult(total, scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(bunny, program, "in_Position", "inNormal", "inTexCoord");
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
	vec3 temp, p;
	mat4 m;

	p.y = prevx-x;
	p.x = -(y-prevy);
	p.z = 0;
	mat3 wv3 = mat4tomat3(worldToViewMatrix);
	p = MultMat3Vec3(wv3, p); //InvertMat3(
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 50.0);
	temp = SetVector(cam_dir.x - cam_pos.x, cam_dir.y - cam_pos.y,cam_dir.z - cam_pos.z);
	cam_dir = VectorAdd(MultMat3Vec3(mat4tomat3(m),temp),cam_pos);
	//cam_up = MultMat3Vec3(TransposeMat3(mat4tomat3(m)),cam_up);
	worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);//cam_up.x,cam_up.y,cam_up.z);//cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	prevx = x;
	prevy = y;
	glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y)
{
		float a = 1.0;
	switch (c)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		v = SetVector(cam_pos.x-cam_dir.x,cam_pos.y - cam_dir.y, cam_pos.z - cam_dir.z);
		vec3 front_vec = SetVector(0,0,-1);
		vec3 front_rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),front_vec);
		cam_pos = SetVector(cam_pos.x +a*front_rot.x,cam_pos.y + a*front_rot.y,cam_pos.z + a*front_rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
		cam_dir = SetVector(cam_dir.x +a*front_rot.x,cam_dir.y + a*front_rot.y,cam_dir.z + a*front_rot.z);
		worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);
		glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program_skynet, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glutPostRedisplay();
		break;
	case 's':
		v = SetVector(cam_pos.x-cam_dir.x,cam_pos.y - cam_dir.y, cam_pos.z - cam_dir.z);
		vec3 back_vec = SetVector(0,0,1);
		vec3 back_rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),back_vec);
		cam_pos = SetVector(cam_pos.x +a*back_rot.x,cam_pos.y + a*back_rot.y,cam_pos.z + a*back_rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
		cam_dir = SetVector(cam_dir.x +a*back_rot.x,cam_dir.y + a*back_rot.y,cam_dir.z + a*back_rot.z);
		worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);
		glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program_skynet, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glutPostRedisplay();
		break;
	case 'a':
		v = SetVector(cam_pos.x-cam_dir.x,cam_pos.y - cam_dir.y, cam_pos.z - cam_dir.z);
		vec3 left_vec = SetVector(-1,0,0);
		vec3 left_rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),left_vec);
		cam_pos = SetVector(cam_pos.x +a*left_rot.x,cam_pos.y + a*left_rot.y,cam_pos.z + a*left_rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
		cam_dir = SetVector(cam_dir.x +a*left_rot.x,cam_dir.y + a*left_rot.y,cam_dir.z + a*left_rot.z);//-
		worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);
		glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program_skynet, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glutPostRedisplay();
		break;
	case 'd':
		v = SetVector(cam_pos.x-cam_dir.x,cam_pos.y - cam_dir.y, cam_pos.z - cam_dir.z);
		vec3 right_vec = SetVector(1,0,0);
		vec3 right_rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),right_vec);
		cam_pos = SetVector(cam_pos.x +a*right_rot.x,cam_pos.y + a*right_rot.y,cam_pos.z + a*right_rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
		cam_dir = SetVector(cam_dir.x +a*right_rot.x,cam_dir.y + a*right_rot.y,cam_dir.z + a*right_rot.z);
		worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);
		glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program_skynet, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
		glutPostRedisplay();
		break;
	}
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("GL3 triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseDragged);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
	return 0;
}
