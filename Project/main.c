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
#include "skybox.h"
#include "terrain.h"
#include "light_sources.h"
//#include "particlesystem.h"

//"Globals"
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir,cam_speed;
GLint prevx,prevy;

//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading, particle_shading;
//Initialize Model stuff
//WorldObject *octagon, *skybox, *ground, *bunny;
TerrainObject *terrain_l,*terrain_h,*terrain_above;
SkyBoxObject *skybox[6], *test_skybox;
ObjectList *created_objects;
ModelList *models;
TextureList *textures;
LightSources *lights;

unsigned int particleVAO;
int height = 5, num_particles = 300, width = 4;
GLfloat t = 0;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	//Load Shaders
	load_shaders(&basic_shading,&skybox_shading, &advanced_shading,&particle_shading);	// Load and compile shader
	//Camera init
	camera_init(&cam_pos,&cam_dir,&projectionMatrix,&worldToViewMatrix);
	//Lights init
	lights = lighting_hell();
	//Skybox init
	make_skybox_object(skybox, "Textures/dirt.tga", "Textures/skybox/sunset2.tga", "Textures/skybox/sunset3.tga", "Textures/skybox/sunset4.tga", "Textures/skybox/sunset5.tga", "Textures/skybox/sunset6.tga");
	//Terrain and other models init
	terrain_l = new_terrain("Textures/kt_rock_1f_dk.tga", T(0,0,0), 100);
	terrain_h = new_terrain("Textures/kt_rock_1f_dk.tga", T(0,0,0), -100);
	models = load_models();
	test_skybox = skybox[0];
	//Textures init
	textures = load_textures();
	//Create objects
	created_objects = create_objects(textures,models);
	init_particles(&particle_shading, &particleVAO, num_particles, width, height);


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
	render_skybox(skybox, worldToViewMatrix, &projectionMatrix, &skybox_shading);
	//Draw everything else, begin with applying lighting to shaders
	apply_lighting(lights, &advanced_shading, cam_pos);
	apply_lighting(lights, &particle_shading, cam_pos);
	render_terrain(terrain_l, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_terrain(terrain_h, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_objects(created_objects, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	mat4 pos;
	pos = T(0.1,10,10);
	t += 0.02;
	render_particles(&pos, &worldToViewMatrix, &projectionMatrix, &particle_shading, &particleVAO, num_particles, width, height, t);

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
