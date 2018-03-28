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
#include "objects.h"

//"Globals"
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir,cam_speed;
GLint prevx,prevy;
//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading;
//Initialize Model stuff
Model *octa;
//Make objects instead
WorldObject *object, *skybox, *ground;
void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	//Load Shaders
	load_shaders(&basic_shading,&skybox_shading, &advanced_shading);	// Load and compile shader
	//Camera init
	camera_init(&cam_pos,&cam_dir,&projectionMatrix,&worldToViewMatrix);

	//LoadTGATextureSimple("Textures/SkyBox512.tga", &skyTex);
	//Make world object cause this is much cooler. Constructor which Loads the associated TGA-file, object file and position.

	object = new_object("Textures/maskros512.tga", "Models/octagon.obj", T(0,0,10));
	ground = new_object("Textures/lava.tga", "Models/ground.obj", T(0,0,0));
	skybox = new_object("Textures/SkyBox512.tga", "Models/skybox.obj", T(0,0,0));
	glutPostRedisplay();
}

void timer(int i)
{
	glutTimerFunc(10, &timer, i);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the skybox

	glDisable(GL_DEPTH_TEST);
	/*
	glUseProgram(skybox_shading);
	//TODO dont bind every time, only bind when it is necessary
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyTex);
	glUniform1i(glGetUniformLocation(skybox_shading, "texUnit"), GL_TEXTURE1);
	modelToWorldMatrix = T(cam_pos.x,cam_pos.y,cam_pos.z);
	totMatrix = Mult(projectionMatrix,Mult(worldToViewMatrix,modelToWorldMatrix));
	glUniformMatrix4fv(glGetUniformLocation(skybox_shading, "totMatrix"), 1, GL_TRUE, totMatrix.m);
	DrawModel(skybox, skybox_shading, "inPosition", "inNormal", "inTexCoord");
	*/
	render_object(skybox, worldToViewMatrix, &projectionMatrix, &skybox_shading, 0);

	glEnable(GL_DEPTH_TEST);
	//Draw using object container
	render_object(ground, worldToViewMatrix, &projectionMatrix, &advanced_shading, 1);
	render_object(object, worldToViewMatrix, &projectionMatrix, &advanced_shading, 1);

	glutSwapBuffers();
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
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1920, 1080);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init();

  //TODO Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(10, &timer, 0);

	glutMainLoop();
	exit(0);
}
