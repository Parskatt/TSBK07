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
#include "terrain.h"


#include "light_sources.h"
//"Globals"
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir,cam_speed;
GLint prevx,prevy;
//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading;
//Initialize Model stuff
<<<<<<< HEAD
//Make objects instead
//WorldObject *octagon, *skybox, *ground, *bunny;
TerrainObject *terrain_l,*terrain_h,*terrain_above;
WorldObject *skybox;
ObjectList *created_objects;
ModelList *models;
TextureList *textures;
LightSources *lights;

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
	//Lights init
	lights = lighting_hell();
	//Skybox init
	skybox = new_skybox("Textures/SkyBox512.tga", "Models/skybox.obj", T(0,0,0));
	//Terrain and other models init
	terrain_l = new_terrain("Textures/kt_rock_1f_dk.tga", T(0,0,0), 100);
	terrain_h = new_terrain("Textures/kt_rock_1f_dk.tga", T(0,0,0), -100);
	models = load_models();
	//Textures init
	textures = load_textures();
	//Create objects
	created_objects = create_objects(textures,models);
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
<<<<<<< HEAD

	//Draw the skybox
	glDisable(GL_DEPTH_TEST);
	render_skybox(skybox, worldToViewMatrix, &projectionMatrix, &skybox_shading);
	glEnable(GL_DEPTH_TEST);
	//Draw everything else, begin with applying lighting to shaders
	apply_lighting(lights, &advanced_shading, cam_pos);
	render_terrain(terrain_l, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_terrain(terrain_h, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_objects(created_objects, &worldToViewMatrix, &projectionMatrix, &advanced_shading);

	//Swap buffers to get new stuff out, redisplay to init next update.
	glutSwapBuffers();
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
