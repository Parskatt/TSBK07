#include "cameramovement.h"

TextureData ttex;
vec3 portal_cave, portal_above;
bool teleported = false;


void camera_init(vec3 *cam_pos,vec3 *cam_dir,mat4 *projectionMatrix,mat4 *worldToViewMatrix)
{
	*cam_pos = SetVector(0, 0, 0);
	*cam_dir = SetVector(0, 0, 10);
	*worldToViewMatrix = lookAt(cam_pos->x, cam_pos->y, cam_pos->z, cam_dir->x, cam_dir->y, cam_dir->z, 0,1,0);
	*projectionMatrix = frustum(-0.16, 0.16, -0.09, 0.09, 0.2, 1000.0);
	LoadTGATextureData("Textures/above_terrain.tga", &ttex);
	portal_cave = SetVector(251, 25, 251); //x coord , radius, z coord of center of portal
	portal_above = SetVector(0, 90, 0); //where to land in the above
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

void keyboard(unsigned char c, int x, int y, mat4* worldToViewMatrix, vec3* cam_pos, vec3* cam_dir, GLuint* torch_flag, GLuint* on_ground)
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
	case 'l':
		*torch_flag = 1;
		break;
	case 'c':
	if (*on_ground == 0)
	{
		*on_ground = 1;
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
	 if (*on_ground == 0) {
		 if ((pow((cam_pos->x+a*rot.x - portal_cave.x), 2) + pow(cam_pos->z + a*rot.z - portal_cave.z, 2)) < portal_cave.y && teleported == false)
		 {
			 *cam_pos = SetVector(portal_above.x,portal_above.y,portal_above.z);
			 *cam_dir = SetVector(portal_above.x,portal_above.y,10);
	 		 *worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
				teleported = true;
		 }
		//*cam_speed = SetVector(cam_speed->x +a*rot.x,cam_speed->y + a*rot.y,cam_speed->z + a*rot.z);
		*cam_pos = SetVector(cam_pos->x +a*rot.x,cam_pos->y + a*rot.y,cam_pos->z + a*rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
		*cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y + a*rot.y,cam_dir->z + a*rot.z);
		*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
		printf("cam_pos = %f %f %f\n", cam_pos->x, cam_pos->y, cam_pos->z);
	}
	else {
		vec3 transvec = translate(cam_pos->x, cam_pos->y, cam_pos->z, &ttex);
		*cam_pos = SetVector(cam_pos->x +a*rot.x,55 + cam_pos->y - transvec.y ,cam_pos->z + a*rot.z);
		*cam_dir = SetVector(cam_dir->x +a*rot.x,55 + cam_dir->y - transvec.y ,cam_dir->z + a*rot.z);
		*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
		printf("cam_pos = %f %f %f\n", cam_pos->x, cam_pos->y, cam_pos->z);
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
