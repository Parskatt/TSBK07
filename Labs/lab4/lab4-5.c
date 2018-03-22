// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix,worldToViewMatrix;
vec3 cam_pos,cam_dir,v;
GLint prevx,prevy;
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


// vertex array object
Model *m, *m2, *tm, *octa;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);

	// Load and compile shader
	program = loadShaders("terrain5.vert", "terrain5.frag");
	glUseProgram(program);
	printError("init shader");

	cam_pos = SetVector(0, 0, 30);
	cam_dir = SetVector(0, 0, 20);
	worldToViewMatrix = lookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("../lab3/dirt.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
	octa = LoadModelPlus("octagon.obj");

	glutPostRedisplay();
}

vec3 translate(double x, double y, double z, TextureData *tex)
{
	//printf("i translate\n");
	int ceilx, ceilz, floorx, floorz;
	vec3 y0, y1, y2;
	ceilx = (int)ceil(x);
	ceilz = (int)ceil(z);
	floorx = (int)floor(x);
	floorz = (int)floor(z);

	if(ceilx == floorx){
		ceilx += 1;
	}
	if(ceilz == floorz){
		ceilz += 1;
	}
	//printf("xfloor %d xceil%d osv%d %d\n",floorx,ceilx,floorz,ceilz);
	y0 = SetVector(ceilx, tex->imageData[(ceilx + floorz * tex->width) * (tex->bpp/8)] / 10.0, floorz);
	y2 = SetVector(floorx, tex->imageData[(floorx + ceilz * tex->width) * (tex->bpp/8)] / 10.0, ceilz);

	if ((x-floorx) + (z - floorz) > 0.5)
			y1 = SetVector(ceilx, tex->imageData[(ceilx + ceilz * tex->width) * (tex->bpp/8)] / 10.0, ceilz);
	else
			y1 = SetVector(floorx, tex->imageData[(floorx + floorz * tex->width) * (tex->bpp/8)] / 10.0, floorz);

	vec3 p1 = VectorSub(y2, y1);
	vec3 p2 = VectorSub(y0, y1);
	vec3 pn = Normalize(CrossProduct(p1,p2));
	//printf("p1 %1f %1f %1f \n", p1.x,p1.y,p1.z);
	double D = DotProduct(pn, y1);

	double dist = DotProduct(pn, SetVector(x,y,z)) - D;
	//printf("dist %1f\n", dist);
	//octagon y - vertex y
	return ScalarMult(pn, dist);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelworld;

	printError("pre display");

	glUseProgram(program);

	// Build matrix
	/*
	vec3 cam = {0, 0, 1};
	vec3 lookAtPoint = {0, 40, 0};

	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	*/
	modelworld = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelworld.m);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	//octagon model
	vec3 transvec = translate(xobj, yobj, zobj, &ttex);
	yobj -= transvec.y;
	printf("trans %1f %1f %1f\n", transvec.x, transvec.y, transvec.z);
	total = T(xobj, yobj, zobj);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(octa, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) // not removed int button
{
	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
	}
	glutPostRedisplay();
	//printf("%d %d\n", x, y);
}


void mouseDragged(int x, int y)
{
	vec3 temp, p={0.0,0.0,0.0};
	mat4 m;

	p.y = prevx-x;
	p.x = -(y-prevy);
	p.z = 0;

	//printf("Prevx: %d \n",prevx);
	//printf("X: %d \n", x);
	//printf("Difference: %d \n", p.y);

	mat3 wv3 = TransposeMat3(mat4tomat3(worldToViewMatrix));
	p = MultMat3Vec3(wv3, p); //InvertMat3(
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 50.0);
	temp = SetVector(cam_dir.x - cam_pos.x, cam_dir.y - cam_pos.y,cam_dir.z - cam_pos.z);

	cam_dir = VectorAdd(MultMat3Vec3(mat4tomat3(m),temp),cam_pos);
	worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);//cam_dir.x, cam_dir.y, cam_dir.z, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

	prevx = x;
	prevy = y;
	glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y)
{
	float a = 1;
	vec3 rot = SetVector(0,0,0);
	vec3 front_vec =  SetVector(0,0,-1);
	vec3 back_vec = SetVector(0,0,1);
	vec3 left_vec = SetVector(-1,0,0);
	vec3 right_vec = SetVector(1,0,0);
	v = SetVector(cam_pos.x-cam_dir.x,cam_pos.y - cam_dir.y, cam_pos.z - cam_dir.z);
	switch (c)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),front_vec);
		break;
	case 's':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),back_vec);
		break;
	case 'a':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),left_vec);
		break;
	case 'd':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(worldToViewMatrix)),right_vec);
		break;
	case GLUT_KEY_UP :
		xobj += 0.1;
		break;
	case GLUT_KEY_DOWN :
		xobj -= 0.1;
		break;
	case GLUT_KEY_RIGHT :
		zobj += 0.1;
		break;
	case GLUT_KEY_LEFT :
		zobj -= 0.1;
		break;

	}
	cam_pos = SetVector(cam_pos.x +a*rot.x,cam_pos.y + a*rot.y,cam_pos.z + a*rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
	cam_dir = SetVector(cam_dir.x +a*rot.x,cam_dir.y + a*rot.y,cam_dir.z + a*rot.z);
	worldToViewMatrix = lookAt(cam_pos.x,cam_pos.y,cam_pos.z, cam_dir.x,cam_dir.y,cam_dir.z,0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseDragged);
	glutKeyboardFunc(keyboard);

	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
