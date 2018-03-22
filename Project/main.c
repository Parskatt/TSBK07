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

//Globals
mat4 projectionMatrix, worldToViewMatrix, modelToWorldMatrix, totMatrix;
vec3 cam_pos,cam_dir;
GLint prevx,prevy;
//Initialize Shading stuff
GLuint basic_shading, skybox_shading, advanced_shading, tex1;
//Initialize Model stuff
Model *octa,*tm;
//GLuint maskros_tex;
TextureData ttex; // terrain
double xobj = 0.0, zobj = 0.0,yobj = 20.0;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	vec3 p0 = SetVector(0.0,0.0,0.0);
	vec3 p1 = SetVector(0,0,0);
	vec3 p2 = SetVector(0,0,0);
	vec3 n = SetVector(0,1,0);

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	printf("triangles: %d\n", triangleCount);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

			if (x > 0 && z > 0)
			{
				p0 = SetVector((x-1.0)/1.0, tex->imageData[(x-1 + z * tex->width) * (tex->bpp/8)] / 20.0, z / 1.0);
				p1 = SetVector((x+1.0)/1.0, tex->imageData[(x+1 + z * tex->width) * (tex->bpp/8)] / 20.0, z / 1.0);
				p2 = SetVector(x/1.0, tex->imageData[(x + (z-1) * tex->width) * (tex->bpp/8)] / 20.0, (z-1) / 1.0);
				n = CrossProduct(VectorSub(p2, p1), VectorSub(p0, p1));
				n = Normalize(n);
			}
			else
			{
				n = SetVector(0.0, 1.0, 0.0);
			}

			normalArray[(x + z * tex->width)*3 + 0] = n.x; //0.0;
			normalArray[(x + z * tex->width)*3 + 1] = n.y;//n.y; //1.0;
			normalArray[(x + z * tex->width)*3 + 2] = n.z; //0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);

	//Load Shaders
	load_shaders(&basic_shading);	// Load and compile shader
	glUseProgram(basic_shading);
	printError("init shader");
	cam_pos = SetVector(0, 0, 30);
	cam_dir = SetVector(0, 0, 20);
	worldToViewMatrix = lookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	modelToWorldMatrix = T(0,0,10);

	glUniformMatrix4fv(glGetUniformLocation(basic_shading, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(basic_shading, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(basic_shading, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("Textures/maskros512.tga", &tex1);

	// Load models
	LoadTGATextureData("Textures/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
	octa = LoadModelPlus("Models/octagon.obj");

  printError("init");
	glutPostRedisplay();
}

void timer(int i)
{
	glutPostRedisplay();
	glutTimerFunc(20, &timer, i);
}

void display(void)
{
	mat4 modelworld;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(basic_shading); //program used when drawing octagon
	totMatrix = Mult(projectionMatrix,Mult(worldToViewMatrix,modelToWorldMatrix));

	//World
	modelworld = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(basic_shading, "totMatrix"), 1, GL_TRUE, modelworld.m);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, basic_shading, "inPosition", "inNormal", "inTexCoord");

	//octagon model
	//glUniformMatrix4fv(glGetUniformLocation(basic_shading, "totMatrix"), 1, GL_TRUE, totMatrix.m);
	//DrawModel(octa, basic_shading, "inPosition", NULL, "inTexCoord");


	printError("display");
	glutSwapBuffers();
}

void mousefunc(int button, int state, int x, int y)
{
  mouse(button, state, x, y, &prevx, &prevy);
	glutPostRedisplay();
}

void mouseDraggedfunc(int x, int y)
{
  mouseDragged(x, y, &prevx, &prevy, &worldToViewMatrix, &cam_dir, &cam_pos);
	glutPostRedisplay();
}

void keyboardfunc(unsigned char c, int x, int y)
{
  keyboard(c, x, y, &worldToViewMatrix, &cam_pos, &cam_dir);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init();

  //TODO Make into one func
	glutMouseFunc(&mousefunc);
	glutMotionFunc(&mouseDraggedfunc);
	glutKeyboardFunc(&keyboardfunc);

	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
