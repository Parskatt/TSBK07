#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include <stdio.h>
#include <stdlib.h>
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

//"Globals"
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir,cam_speed;
vec3 ring_pos,doom_pos;
GLint prevx,prevy;

//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading, splat_shading, particle_shading, torch_flag=0, on_ground = 0, in_cave = 1, has_ring = 0;
//Initialize Model stuff
//Make objects instead
TerrainObject *terrain_l,*terrain_h;
SplatTerrain *terrain_above;
SkyBoxObject *skybox[6];
ObjectList *created_static_objects,*torches;
ModelList *models;
TextureList *textures;
LightSources *lights;

unsigned int particleVAO;
int height = 55, num_particles = 400, width = 5;
GLfloat t = 500;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");
	doom_pos = SetVector(114,74,124);
	//Load Shaders
	load_shaders(&basic_shading,&skybox_shading, &advanced_shading, &splat_shading, &particle_shading);	// Load and compile shader
	//Camera init
	camera_init(&cam_pos,&cam_dir,&projectionMatrix,&worldToViewMatrix);
	//Lights init
	lights = lighting_hell();
	//Skybox init
	//left, right, up, down, front, back
	make_skybox_object(skybox, "Textures/skybox/miramar_lf.tga","Textures/skybox/miramar_rt.tga","Textures/skybox/miramar_up.tga","Textures/skybox/miramar_dn.tga","Textures/skybox/miramar_ft.tga","Textures/skybox/miramar_bk.tga");//"Textures/skybox/sunset1.tga", "Textures/skybox/sunset3.tga", "Textures/skybox/sunset5.tga", "Textures/skybox/sunset4.tga", "Textures/skybox/sunset3.tga", "Textures/skybox/sunset6.tga");
	//Terrain and other models init
	terrain_l = new_terrain("Textures/kt_rock_1f_dk.tga", "Textures/preliminary_cave_design3.tga", T(0,0,0), 100);
	terrain_h = new_terrain("Textures/kt_rock_1f_dk.tga", "Textures/preliminary_cave_design3.tga", T(0,0,0), -100);

	models = load_models();
	//Textures init
	textures = load_textures();
	terrain_above = new_splat("Textures/lava.tga","Textures/grass.tga","Textures/berg2.tga", "Textures/above_terrain.tga", "Textures/map.tga", T(0,50,0), 10);

	//Create objects
	ring_pos = SetVector(222,0,228);
	created_static_objects = create_static_objects(textures,models);
	torches = create_torch_objects();
	init_particles(&particle_shading, &particleVAO, num_particles, width, height);
	glutPostRedisplay();
	system("clear");
}

void timer(int i)
{
	glutTimerFunc(10, &timer, i);
}

void display(void)
{

	//Check if a torch should be added
	if(torch_flag){
		append_ptlight(lights,make_point_light(cam_pos,0.5,0.01,0.02,SetVector(1.0,0.5,0),SetVector(1,0.5,0),SetVector(1,0.5,0)));
		add_torch(torches,textures,models, cam_pos);
		torch_flag = 0;
	}
	if(distance(ring_pos,cam_pos) < 3.0){
		has_ring = 1;
	}
	if(has_ring && distance(cam_pos,doom_pos) < 3.0 ){
		system("clear");
		printf("\n***Congratulations!****\n");
		exit(0);
	}
	//printf("\r has_ring: %f %f %f",cam_pos.x,cam_pos.y,cam_pos.z);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Draw the skybox
	render_skybox(skybox, worldToViewMatrix, &projectionMatrix, &skybox_shading);
	//Draw everything else, begin with applying lighting to shaders
	apply_lighting(lights, &advanced_shading, cam_pos);
	apply_lighting(lights, &splat_shading, cam_pos);
	apply_lighting(lights, &particle_shading, cam_pos);
	render_terrain(terrain_l, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_terrain(terrain_h, &worldToViewMatrix, &projectionMatrix, &advanced_shading);
	render_splat(terrain_above, &worldToViewMatrix, &projectionMatrix, &splat_shading);
	render_objects(created_static_objects, &worldToViewMatrix, &projectionMatrix, &advanced_shading,has_ring);
	render_objects(torches, &worldToViewMatrix, &projectionMatrix, &advanced_shading,0);

	mat4 pos;
	pos = T(125,50,125);
	t += 0.1;

	//Bind om texturen!
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
  keyboard(c, x, y, &worldToViewMatrix, &cam_pos, &cam_dir, &torch_flag, &on_ground, &in_cave);
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
