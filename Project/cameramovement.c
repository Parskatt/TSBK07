#include "cameramovement.h"

TextureData above_tex;
TextureData cave_tex;

vec3 portal_cave, portal_above;
bool teleported = false;


void camera_init(vec3 *cam_pos,vec3 *cam_dir,mat4 *projectionMatrix,mat4 *worldToViewMatrix)
{
	*cam_pos = SetVector(10, -1, 10);
	*cam_dir = SetVector(0, 0, 10);
	*worldToViewMatrix = lookAt(cam_pos->x, cam_pos->y, cam_pos->z, cam_dir->x, cam_dir->y, cam_dir->z, 0,1,0);
	*projectionMatrix = frustum(-0.16, 0.16, -0.09, 0.09, 0.2, 1000.0);
	LoadTGATextureData("Textures/above_terrain.tga", &above_tex);
	LoadTGATextureData("Textures/preliminary_cave_design3.tga", &cave_tex);
	portal_cave = SetVector(248, 25, 248); //x coord , radius, z coord of center of portal
	portal_above = SetVector(35, 53, 35); //where to land in the above
}

vec3 translate(double x, double y, double z, TextureData *tex, int height_scale, int ytrans)
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
	//printf("What does texture high say: %d, What pos are we: %f\n",tex->imageData[(ceilx + floorz * tex->width) * (tex->bpp/8)]/ height_scale,y);
	y0 = SetVector(ceilx, tex->imageData[(ceilx + floorz * tex->width) * (tex->bpp/8)] / height_scale + ytrans, floorz);
	y2 = SetVector(floorx, tex->imageData[(floorx + ceilz * tex->width) * (tex->bpp/8)] / height_scale + ytrans, ceilz);

	if ((x-floorx) + (z - floorz) > 0.5)
			y1 = SetVector(ceilx, tex->imageData[(ceilx + ceilz * tex->width) * (tex->bpp/8)] / height_scale + ytrans, ceilz);
	else
			y1 = SetVector(floorx, tex->imageData[(floorx + floorz * tex->width) * (tex->bpp/8)] / height_scale + ytrans, floorz);

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

void keyboard(unsigned char c, int x, int y, mat4* worldToViewMatrix, vec3* cam_pos, vec3* cam_dir, GLuint* torch_flag, GLuint* on_ground, GLuint* in_cave)
{
	static float a = 0.5;
	vec3 direction = SetVector(0,0,0);
	switch (c)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		direction = SetVector(0,0,-1);
		break;
	case 's':
		direction = SetVector(0,0,1);
		break;
	case 'a':
		direction = SetVector(-1,0,0);
		break;
	case 'd':
		direction = SetVector(1,0,0);
		break;
	case 'f': //For fullscreen
		glutToggleFullScreen();
		break;
	case 32:
		*torch_flag = 1;
		break;
	case 'c':
	if (*in_cave == 0)
	{
		*in_cave = 1;
		*on_ground = 0;
	}
	else
	{
		*in_cave = 0;
	}
		break;
	case 'g':
	if (*on_ground == 0)
		{
			*on_ground = 1;
			*in_cave = 0;
		}
	else
		{
			*on_ground = 0;
		}
		break;
	default:
		direction = SetVector(0,0,0);
		//a = 0.0;
		break;
	}

	vec3 rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),direction);

	 if (*on_ground == 1) {
		 vec3 transvec = translate(cam_pos->x+a*rot.x, cam_pos->y+a*rot.y, cam_pos->z+a*rot.y, &above_tex, 10, 52); //following ground in the above world
		 if ((cam_pos->y - cam_pos->y+a*rot.y-transvec.y) < 5 && (cam_pos->y - cam_pos->y+a*rot.y-transvec.y) > -5)
		 {
			 *cam_pos = SetVector(cam_pos->x+a*rot.x,cam_pos->y +a*rot.y- transvec.y ,cam_pos->z +a*rot.z); //add 55 since above terrain is translated 50
			 *cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y - transvec.y ,cam_dir->z + a*rot.z);
			 *worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
		 }
	}

	else if (*in_cave == 1)
	{
		vec3 transvec = translate(cam_pos->x, cam_pos->y, cam_pos->z, &cave_tex, -100,1); //following ground in cave
		//printf("Nu är vi här\n");
		//printf("ny y%f\n",(cam_pos->y+a*rot.y-transvec.y));
		if ((cam_pos->y+a*rot.y-transvec.y) < -0.2 )
		{
			*cam_pos = SetVector(cam_pos->x+a*rot.x,cam_pos->y +a*rot.y- transvec.y ,cam_pos->z +a*rot.z); //add 55 since above terrain is translated 50
			*cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y - transvec.y ,cam_dir->z + a*rot.z);
			*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
			//printf("Nu är vi inne i loopen \n");
		}
		else
		{
			*cam_pos = SetVector(cam_pos->x, cam_pos->y ,cam_pos->z);
			*cam_dir = SetVector(cam_dir->x, -100 ,cam_dir->z);
			*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
		}

		if ((pow((cam_pos->x+a*rot.x - portal_cave.x), 2) + pow(cam_pos->z + a*rot.z - portal_cave.z, 2)) < portal_cave.y && teleported == false)
		{
			*cam_pos = SetVector(portal_above.x,portal_above.y,portal_above.z);
			*cam_dir = SetVector(portal_above.x,portal_above.y,10);
			*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
			 teleported = true;
			 *in_cave = 0;
			 *on_ground = 1;
			 printf("cave: %d\n", *in_cave);
		}

		//printf("cam_pos.x = %f, cam_pos.y = %f, cam_pos.z = %f\n", cam_pos->x, cam_pos->y, cam_pos->z);
	}

	else {

	 //*cam_speed = SetVector(cam_speed->x +a*rot.x,cam_speed->y + a*rot.y,cam_speed->z + a*rot.z);
	 *cam_pos = SetVector(cam_pos->x +a*rot.x,cam_pos->y + a*rot.y,cam_pos->z + a*rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
	 *cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y + a*rot.y,cam_dir->z + a*rot.z);
	 *worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y, int* prevx, int* prevy)
{
	if (state == GLUT_DOWN)
	{
		*prevx = x;
		*prevy = y;
	}
	glutPostRedisplay();
}

void mouseDragged(int x, int y, int* prevx, int* prevy, mat4* worldToViewMatrix, vec3* cam_dir, vec3* cam_pos)
{
	vec3 temp, p={0.0,0.0,0.0};
	mat4 m;
	p.y = *prevx-x;//Make an orthogonal vector
	p.x = -(y-*prevy);
	p.z = 0;
	mat3 wv3 = TransposeMat3(mat4tomat3(*worldToViewMatrix));//View->World
	p = MultMat3Vec3(wv3, p); //send p to world coordinates
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 200.0);
	temp = SetVector(cam_dir->x - cam_pos->x, cam_dir->y - cam_pos->y,cam_dir->z - cam_pos->z);
	*cam_dir = VectorAdd(MultMat3Vec3(mat4tomat3(m),temp),*cam_pos);
	*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	*prevx = x;
	*prevy = y;
	glutPostRedisplay();
}
